#pragma once
#include "core/particle.h"
#include "core/elemental_particle_resource.h"



#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>


namespace PSS
{
    class ParticleDataContainer
    {   
        protected:
            godot::PackedVector3Array m_positions;
            godot::PackedColorArray m_colors;
            godot::PackedFloat32Array m_sizes;
            
        public:
            




            Particle add_particle(const Ref<ElementalParticleResource> particleTemplate, godot::Vector3 position);
           

            godot::Color get_particle_color(Particle particle) const;
            void set_particle_color(Particle particle, godot::Color color);


            godot::Vector3 get_position(Particle particle) const;
            void set_position(Particle particle, godot::Vector3 color);

            float get_size(Particle particle) const;
            void set_size(Particle particle, float size);


            ParticleDataContainer();
        
            

    };
}   

   