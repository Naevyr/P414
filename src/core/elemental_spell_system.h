#pragma once

#include "core/particle.h"
#include "core/particle_buffer.h"

#include <memory>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/rendering_device.hpp>





namespace PSS
{
    class ElementalSpellSystem : public godot::Node
    {
        GDCLASS(ElementalSpellSystem, godot::Node)
        private:
            godot::Ref<ParticleBuffer> m_particleBuffer;
            

              

        protected:
           static void _bind_methods();
           
           
        public:


           
        
            //Export
            void set_particle_buffer(godot::Ref<ParticleBuffer> particle_buffer);
            godot::Ref<ParticleBuffer> get_particle_buffer() const; 
        
        
        
            ElementalSpellSystem();


    };
} 
