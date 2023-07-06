#include "rendering/particle_gpu_encoder.h"


#include <godot_cpp/variant/vector4.hpp>
#include <algorithm>

using namespace PSS;
using namespace godot;


PackedByteArray ParticleGPUEncoder::encode_buffer_ordered (Ref<ParticleBuffer> buffer,godot::Vector3 cameraPosition)
{
    PackedByteArray bytes = PackedByteArray();
    bytes.resize(4);
    bytes.resize(16);
    ParticleDataContainer & data = buffer->get_data();
    std::vector<Particle> ordered = std::vector<Particle>(buffer->get_data().get_particles());

    std::sort(ordered.begin(),ordered.end(), [data,cameraPosition](Particle a, Particle b) {
        // You can use 'arg' within the lambda expression
        return data.get_position(a).distance_to(cameraPosition) > data.get_position(b).distance_to(cameraPosition);
        
        return data.get_position(a).distance_to(cameraPosition) < data.get_position(b).distance_to(cameraPosition);
    });
    
 

    

 
   
    bytes.encode_u32(0,buffer->get_particle_count());

    for (size_t i = 0; i < ordered.size(); i++)
    {
       bytes.append_array(encode_particle(ordered[i],buffer));
    }

    return bytes;
    
}


godot::PackedByteArray ParticleGPUEncoder::encode_particle(Particle particle,Ref<ParticleBuffer> buffer)
{
    PackedByteArray bytes = PackedByteArray();
    bytes.resize(32);
    bytes.encode_var(0,buffer->get_data().get_position(particle));
    Vector3 position = buffer->get_data().get_position(particle);
    bytes.encode_float(0,position.x);
    bytes.encode_float(4,position.y);
    bytes.encode_float(8,position.z);
    bytes.encode_float(12, buffer->get_data().get_size(particle));

    Color color =  buffer->get_data().get_particle_color(particle);
    bytes.encode_var(16,Vector3(color.r,color.g,color.b));
    bytes.encode_float(16,color.r);
    bytes.encode_float(20,color.g);
    bytes.encode_float(24,color.b);


    //Padding
    bytes.encode_s32(28,0);
    
    
    
    return bytes;
    
}


godot::PackedByteArray ParticleGPUEncoder::encode_camera(Camera3D * camera)
{
    PackedByteArray bytes = PackedByteArray();
    bytes.resize(96);
    bytes.encode_var(0,camera->get_transform());
    bytes.encode_var(48,Vector4(0,0,0,1));
    Transform3D  transform = camera->get_transform();
    

    //Encode variant exists but for some reason doesn't seem to use the formatting we need, as such manual formatting is needed
    int ind = 0;
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            bytes.encode_float(ind,transform.basis.rows[i][j]);
            ind += 4;
        }
       
        bytes.encode_float(ind,transform.origin[i]);
        ind += 4;
    }

    bytes.encode_float(48,0);
    bytes.encode_float(52,0);
    bytes.encode_float(56,0);
    bytes.encode_float(60,1);


    bytes.encode_float(64,camera->get_fov());
    //éPadding
    bytes.encode_var(68,Vector3());;

    return bytes;;
    
}


