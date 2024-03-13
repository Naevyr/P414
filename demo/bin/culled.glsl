#[compute]
#version 460


#define N_RAY_KERNEL_SIZE 128

//#define N_TOTAL_KERNEL_SIZE  N_RAY_KERNEL_SIZE_AXIS * N_RAY_KERNEL_SIZE_AXIS * N_RAY_KERNEL_SIZE_AXIS
#define M_PI 3.1415926535897932384626433832795
// Invocations in the (x, y, z) dimension
layout(local_size_x = N_RAY_KERNEL_SIZE) in;



layout (binding = 1,  std430) restrict readonly buffer ParticleBuffer
{
    uint number_of_particles;
    uint[] particles;
}
particle_buffer;


layout (binding = 2,  std430) restrict readonly buffer PositionParticleBuffer
{
    vec3[] positions;
}
position_particle_buffer;


layout (binding = 3,  std430) restrict readonly buffer ColorParticleBuffer
{
    
    vec3[] colors;
}
color_particle_buffer;




layout (binding = 4,  std430) buffer Camera
{
    
    
    layout(row_major) mat4x4 transform;
    float fov;

}
camera;


layout (binding = 5,  std430) buffer ParticleData
{
    float size;
}
particle_data;



layout( rgba32f , binding = 0) uniform writeonly image2D u_densityTexture;

shared uint nearestParticle;


vec3 calculateRayDirection(vec2 uv, mat3x3 cameraTransform, float fov ) {
  
    float fovRad = radians(fov+30);

    vec2 size = gl_NumWorkGroups.xy;
    vec2 coords = gl_WorkGroupID.xy;
    float relativeX =  coords.x * 2 / size.x - 1; // [0, screenWidth] -> [-1, 1]
    float relativeY = -(coords.y * 2 / size.y - 1); // [0, screenHeight] -> [-1, 1], might need to be flipped depending on the coordinate system
    relativeX *= size.x/size.y;
    vec3 viewVector = vec3(relativeX * sin(fovRad / 2),
                       relativeY * sin(fovRad/ 2),
                       -1);

    vec3 rayDirWorld =  viewVector * transpose(cameraTransform);


    return normalize(rayDirWorld);
}
bool intersectSphere(vec3 ray,vec3 cameraPos ,vec3 particle)
{

    vec3 oc =  cameraPos - particle.xyz;

    float a = dot(ray, ray);
    float b = 2.0 * dot(ray,oc);
    float c = dot(oc, oc) - pow( particle_data.size,2.0);
  
    float discriminant = b * b - 4.0 * a * c;
    float t = (- b - sqrt(max(0,discriminant))) / (2.0 * a);

    return  discriminant >= 0.0 && t >= 0;
}

void checkParticles(vec3 ray)
{

    vec3 cameraPos = camera.transform[3].xyz;
    
    for(uint i = gl_LocalInvocationIndex ; i < particle_buffer.number_of_particles; i += N_RAY_KERNEL_SIZE)
    {
        
        if(intersectSphere(ray,cameraPos, position_particle_buffer.positions[particle_buffer.particles[i]]))
        {
            atomicMin(nearestParticle,i);
            return;
        }
    }

}



vec4 rayTracing(vec3 ray)
{

    checkParticles(ray);


    barrier();
    return vec4( color_particle_buffer.colors[particle_buffer.particles[nearestParticle]].rgb, 1 - nearestParticle / particle_buffer.number_of_particles);
   
        
}
   

vec2 uv()
{
    vec2 size = gl_NumWorkGroups.xy;
    vec2 uv = gl_WorkGroupID.xy  / size.xy ;
  
    uv.y = 1-uv.y;
    uv.x *= size.x/size.y;
    
    return uv;
}



void main() {
    
    nearestParticle =  particle_buffer.number_of_particles;
    
    vec3 ray = calculateRayDirection(uv(),mat3(camera.transform), camera.fov);


    barrier();

    vec4 color = rayTracing(ray);

    barrier();
   
    imageStore(u_densityTexture, ivec2(gl_WorkGroupID.xy) , color);
 
    //imageStore(u_densityTexture, ivec2(gl_WorkGroupID.xy * N_RAY_KERNEL_SIZE_AXIS + vec2(gl_LocalInvocationID.x,gl_LocalInvocationID.y ) - N_RAY_KERNEL_SIZE_AXIS / 2),vec4(1));



  
}
