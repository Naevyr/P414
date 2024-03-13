#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/mesh.hpp>

#include "core/elemental_particle_resource.h"


namespace PSS
{
    class ElementalMeshResource : public godot::Resource
    {
        GDCLASS(ElementalMeshResource, godot::Resource)

        
        private:

            godot::Ref<ElementalParticleResource> m_templateParticle;

            godot::PackedVector3Array m_exportPositions;
            godot::PackedVector3Array m_meshNormals;
            godot::PackedVector3Array m_meshPositions;
            
            int m_sampleSize;

            godot::Ref<godot::Mesh> m_convexMesh;
            
        private:
            bool isPointInsideMesh(godot::Vector3);
            void generatePositionsVertically(godot::Vector3 tempPos, bool upwards, float size);
            void generateParticlePositions(godot::Vector3 position, float size);


        
        protected:
        static void _bind_methods();

        
        public:
        ElementalMeshResource();
        void generate_mesh(float size);


        //Export
        void set_position_array(godot::PackedVector3Array array);
        godot::PackedVector3Array get_position_array() const;
        //Export
        void set_convex_mesh(godot::Ref<godot::Mesh> mesh);
        godot::Ref<godot::Mesh> get_convex_mesh() const;
        
        //Export 
        void set_sample_size( int size);
        int get_sample_size() const;

        //Export
        void set_template_particle(godot::Ref<ElementalParticleResource> templateParticle);
        godot::Ref<ElementalParticleResource> get_template_particle() const;


    };
} // namespace P414
