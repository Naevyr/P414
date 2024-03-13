#pragma once


#include "core/particle.h"
#include "core/elemental_particle_resource.h"


#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/box_mesh.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>


#include <queue>
#include <vector>



namespace PSS
{
    class ParticleDataContainer : public godot::Resource
    {   
        GDCLASS(ParticleDataContainer, godot::Resource)
        private:
            std::vector<Particle> m_particles;
            std::vector<Particle> m_culledParticles;


            godot::PackedColorArray m_colors;
            godot::PackedVector3Array m_positions;

            float m_size;

    

        protected:
            static void _bind_methods();
            godot::Ref<godot::BoxMesh> m_bounds;
            size_t particle_count;
        public:
            



            Particle add_particle(const godot::Ref<ElementalParticleResource> particleTemplate, godot::Vector3 position) ;
           

            godot::Color get_particle_color(Particle particle) const;
            void set_particle_color(Particle particle, godot::Color color);


            godot::Vector3 get_position(Particle particle) const;
            void set_position(Particle particle, godot::Vector3 color);

            float get_particle_size() const;
            void set_particle_size(float size);


            godot::Ref<godot::BoxMesh> get_bounds() const;
            void set_bounds(godot::Ref<godot::BoxMesh> bounds);

            size_t get_particle_count() const;
            size_t get_culled_particle_count() const;

            void cull_particles(godot::Vector3 cameraPos);
            const std::vector<Particle>& get_particles() const;

            ParticleDataContainer();

    };
}   

   