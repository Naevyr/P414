#include "rendering/particle_gpu_encoder.h"


#include <godot_cpp/variant/vector4.hpp>
#include <algorithm>

using namespace PSS;
using namespace godot;


PackedByteArray ParticleGPUEncoder::encode_buffer_ordered (Ref<ParticleBuffer> buffer,godot::Vector3 cameraPosition)
{
    PackedByteArray bytes = PackedByteArray();
    std::vector<Particle> ordered = std::vector<Particle>(buffer->get_particles());

    std::sort(ordered.begin(),ordered.end(), [buffer,cameraPosition](Particle a, Particle b) {
        // You can use 'arg' within the lambda expression
        return buffer->get_position(a).distance_to(cameraPosition) > buffer->get_position(b).distance_to(cameraPosition);
    });
    
 

    

 
   
    bytes.encode_u32(0,buffer->get_particle_count());

    for (size_t i = 0; i < ordered.size(); i++)
    {
       bytes.append_array(encode_particle(ordered[i],buffer));
    }
    
}


godot::PackedByteArray ParticleGPUEncoder::encode_particle(Particle particle,Ref<ParticleBuffer> buffer)
{
    PackedByteArray bytes;
    bytes.encode_var(0,buffer->get_position(particle));
    bytes.encode_float(12, buffer->get_size(particle));

    Color color =  buffer->get_particle_color(particle);
    bytes.encode_var(16,Vector3(color.r,color.g,color.b));


    //Padding
    bytes.encode_s32(28,0);
    
    
    
    return bytes;
    
}


godot::PackedByteArray ParticleGPUEncoder::encode_camera(Camera3D * camera)
{
    PackedByteArray bytes;
    bytes.encode_var(0,camera->get_transform());
    bytes.encode_var(48,Vector4(0,0,0,1));
    bytes.encode_float(64,camera->get_fov());
    //éPadding
    bytes.encode_var(68,Vector3());;

    return bytes;;
    
}


