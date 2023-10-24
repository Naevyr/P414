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


    class ParticleBuffer : public godot::Resource
    {   
        
        typedef unsigned int Particle;
        

     
        GDCLASS(ParticleBuffer, godot::Resource)
        private:
            
            

        
            unsigned int m_particleCount = 0;
            ParticleDataContainer m_data;
            LinearOctree m_octree;
            


            int m_octanParticleCount = 50;
            int m_octanMaxDepth = 700;


            godot::Ref<godot::BoxMesh> m_bounds;


        protected:


            static void _bind_methods();

        public:

            
            std::vector<Particle> get_nearby_particles(godot::Vector3 position, float size) const;
            
            
            Particle add_particle(const godot::Ref<ElementalParticleResource> particleTemplate, godot::Vector3 position);
            void add_cluster(const godot::Ref<ElementalParticleResource> particleTemplate, const godot::PackedVector3Array & positions);
            void add_mesh(const godot::Ref<ElementalMeshResource> mesh);


            //Export
            unsigned int get_particle_count() const;
           

            //Export
            void set_bounds(godot::Ref<godot::BoxMesh> convexBounds);
            godot::Ref<godot::BoxMesh> get_bounds() const;


            void set_octan_particle_count(int octanParticleCount);
            int get_octan_particle_count() const;

            void set_octan_max_depth(int octanMaxDepth);
            int get_octan_max_depth() const;





            ParticleDataContainer & get_data();



            const LinearOctree & get_octree() const;

            
            ParticleBuffer();
           

    };


  

} // namespace PSS
