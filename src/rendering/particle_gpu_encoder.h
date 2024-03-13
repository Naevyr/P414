#pragma once

#include "core/particle.h"
#include "core/particle_buffer.h"
#include <godot_cpp/classes/texture3d.hpp>

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/camera3d.hpp>
namespace PSS
{

    class ParticleGPUEncoder
    {
        private:
            ParticleGPUEncoder() {};
        //FIXME: Sort only the elements changed, and do not copy everytime a new array, this is unoptimal
        /// <summary>
        /// Ordered by distance to point
        /// </summary>
        public: 
        static godot::PackedByteArray encode_octree_particle_buffer (godot::Ref<ParticleBuffer> buffer);
        static godot::PackedByteArray encode_octree_octans(godot::Ref<ParticleBuffer> buffer);

        static godot::PackedByteArray encode_particle_positions (godot::Ref<ParticleBuffer> buffer);
        static godot::PackedByteArray encode_particle_colors (godot::Ref<ParticleBuffer> buffer);


        static godot::PackedByteArray encode_particle_ordered (godot::Ref<ParticleBuffer> buffer,godot::Vector3 cameraPosition);
        
        
        static godot::PackedByteArray encode_particle(Particle particle,godot::Ref<ParticleBuffer> buffer);
        static godot::PackedByteArray encode_camera(godot::Camera3D * camera);
      
        
        
        
        
    };
}
