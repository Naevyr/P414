#pragma once

#include "core/particle.h"
#include "core/elemental_mesh_resource.h"
#include "core/linear_octree.h"
#include "core/particle_data_container.h"

//Particle buffer, needs to be implemented, works as an interface for gpu and cpu (maybe with the uniform)

#include <vector>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/ref.hpp>



namespace PSS
{
    //Having the buffer as a resource can have a lot of use-cases 
    //It can have multiple instances so you can have multiple particle system working without interfering with each others
    //You can swap between buffer for scene load or stuff like that


    class ParticleBuffer : public godot::Resource, public ParticleDataContainer
    {   
        
        typedef unsigned int Particle;
        

     
        GDCLASS(ParticleBuffer,Resource)
        private:
            
            

        
            unsigned int m_particleCount;
            
            LinearOctree m_octree;
        
            godot::Ref<BoxMesh> m_bounds;


        protected:


            static void _bind_methods();

        public:

            
            std::vector<Particle> get_nearby_particles(godot::Vector3 position, float size) const;
            
            
            Particle add_particle(const godot::Ref<ElementalParticleResource> particleTemplate, godot::Vector3 position);
            std::vector<Particle> add_cluster(const godot::Ref<ElementalParticleResource> particleTemplate, const godot::Ref<godot::PackedVector3Array> positions);

            void add_mesh(const Ref<ElementalMeshResource> mesh);


            //Export
            unsigned int get_particle_count() const;
           


            godot::Ref<BoxMesh> get_convex_bounds() const;
            void set_convex_bounds(godot::Ref<BoxMesh> convexBounds);

            

            
            ParticleBuffer();
           

    };


  

} // namespace PSS
