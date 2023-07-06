#pragma once

#include "core/particle.h"
#include "core/particle_buffer.h"

#include <memory>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/variant/typed_array.hpp>




namespace PSS
{
    class ElementalSpellSystem : public godot::Node
    {
        GDCLASS(ElementalSpellSystem, godot::Node)
        private:
            godot::Ref<ParticleBuffer> m_particleBuffer;
            godot::Ref<ElementalMeshResource> m_debugMeshResource;

              

        protected:
           static void _bind_methods();
           
           
        public:


            void _ready();
        
            //Export
            void set_particle_buffer(godot::Ref<ParticleBuffer> particle_buffer);
            godot::Ref<ParticleBuffer> get_particle_buffer() const; 
            

            void set_debug_mesh_resource(godot::Ref<ElementalMeshResource> meshResource);
            godot::Ref<ElementalMeshResource> get_debug_mesh_resource() const;

        
            ElementalSpellSystem();


    };
} 
