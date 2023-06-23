#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/mesh.hpp>

#include "core/elemental_particle_resource.h"


namespace PSS
{
    class ElementalMeshResource : public godot::Resource
    {
        GDCLASS(ElementalMeshResource,Resource)

        
        private:

            godot::Ref<ElementalParticleResource> m_templateParticle;

            godot::PackedVector3Array m_exportPositions;
            godot::PackedVector3Array m_meshNormals;
            godot::PackedVector3Array m_meshPositions;
            
            size_t m_sampleSize;

            godot::Ref<Mesh> m_convexMesh;
            
        private:
            bool isPointInsideMesh(godot::Vector3);
            void generatePositionsVertically(godot::Vector3 tempPos, bool upwards);
            void generateParticlePositions(godot::Vector3 position);


        
        protected:
        static void _bind_methods();

        
        public:
        ElementalMeshResource();
        void generate_mesh();


        //Export
        godot::Ref<PackedVector3Array> get_position_array() const;
        
        //Export
        godot::Mesh get_convex_mesh();
        void set_convex_mesh(godot::Mesh p_mesh);
        
        //Export 
        unsigned int get_sample_size();
        void set_sample_size( unsigned int p_size);

        //Export
        godot::Ref<ElementalParticleResource> get_template_particle();
        void set_template_particle(godot::Ref<ElementalParticleResource> p_templateParticle);


    };
} // namespace P414
