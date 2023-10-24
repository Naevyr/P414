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
        return data.get_position(a).distance_to(cameraPosition) < data.get_position(b).distance_to(cameraPosition);
        
        
    });
    
 

    

 
   
    bytes.encode_u32(0,buffer->get_particle_count());

    for (size_t i = 0; i < ordered.size(); i++)
    {
       bytes.append_array(encode_particle(ordered[i],buffer));
    }

    return bytes;
    
}


PackedByteArray ParticleGPUEncoder::encode_particle_ordered (Ref<ParticleBuffer> buffer,godot::Vector3 cameraPosition)
{
    PackedByteArray bytes = PackedByteArray();
    bytes.resize(4 + 4 * buffer->get_particle_count());
 
    ParticleDataContainer & data = buffer->get_data();
    std::vector<Particle> ordered = std::vector<Particle>(buffer->get_data().get_particles());

    std::sort(ordered.begin(),ordered.end(), [data,cameraPosition](Particle a, Particle b) {
        // You can use 'arg' within the lambda expression
        return data.get_position(a).distance_to(cameraPosition) < data.get_position(b).distance_to(cameraPosition);
        
        
    });
    
 

    

 
   
    bytes.encode_u32(0,buffer->get_particle_count());

    for (size_t i = 0; i < ordered.size(); i++)
    {
       bytes.encode_u32(4 + i * 4, ordered[i]);
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





godot::PackedByteArray ParticleGPUEncoder::encode_particle_positions (godot::Ref<ParticleBuffer> buffer)
{

    PackedByteArray bytes = PackedByteArray();
    bytes.resize(16 * buffer->get_particle_count());
    for (auto &&particle : buffer->get_data().get_particles())
    {
        
    
        Vector3 position = buffer->get_data().get_position(particle);
        bytes.encode_float(particle * 16, position.x);
        bytes.encode_float(particle * 16 + 4, position.y);
        bytes.encode_float(particle * 16 + 8, position.z);
        bytes.encode_float(particle * 16 + 12, buffer->get_data().get_size(particle));
    
    
    }

    return bytes;
    
}
godot::PackedByteArray ParticleGPUEncoder::encode_particle_colors (godot::Ref<ParticleBuffer> buffer)
{
    PackedByteArray bytes = PackedByteArray();  
    bytes.resize(12 * buffer->get_particle_count());
    for (auto &&particle : buffer->get_data().get_particles())
    {

        Color color = buffer->get_data().get_particle_color(particle);
        bytes.encode_float(particle * 12, color.r);
        bytes.encode_float(particle * 12 + 4, color.g);
        bytes.encode_float(particle * 12 + 8, color.b);

    
    }

    return bytes;
    
}



godot::PackedByteArray ParticleGPUEncoder::encode_octree_particle_buffer (godot::Ref<ParticleBuffer> buffer)
{
    PackedByteArray bytes = PackedByteArray();
    bytes.resize(4 + 4 * buffer->get_particle_count());
    bytes.encode_u32(0,buffer->get_particle_count());


    auto particle_buffer = buffer->get_octree().get_particle_array();
    for (size_t i = 0; i < particle_buffer.size(); i++)
    {
        bytes.encode_u32(4 + 4 * i, particle_buffer[i]);
    }
    
   return bytes;
    

}

godot::PackedByteArray ParticleGPUEncoder::encode_octree_octans(godot::Ref<ParticleBuffer> buffer)
{
    PackedByteArray bytes = PackedByteArray();
    auto octan_buffer = buffer->get_octree().get_octan_data();
    bytes.resize(16 + 32 * octan_buffer.size());
    bytes.encode_u32(0, octan_buffer.size());
    //12 byte padding

    for (size_t i = 0; i < octan_buffer.size(); i++)
    {
        bytes.encode_u32(16 + 32 * i, octan_buffer[i].childCount);

        //4 byte padding
        bytes.encode_u32(16 + 32 * i + 8 ,octan_buffer[i].particleStartIndex);
        bytes.encode_u32(16 + 32 * i + 12, octan_buffer[i].particleCount);
 
        bytes.encode_float(16 + 32 * i + 16, octan_buffer[i].box.position.x);
        bytes.encode_float(16 + 32 * i + 20, octan_buffer[i].box.position.y);
        bytes.encode_float(16 + 32 * i + 24, octan_buffer[i].box.position.z);
        bytes.encode_float(16 + 32 * i + 28, octan_buffer[i].box.size);
 
    }   
    
   return bytes;
    
}
