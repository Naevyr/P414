#pragma once


#include <godot_cpp/classes/node.hpp>
#include "core/particle_buffer.h"


namespace PSS
{
    class ParticlePhysicsSystem : public godot::Node
    {

        private:
            godot::Ref<PSS::ParticleBuffer> m_buffer;
         
          

        GDCLASS(ParticlePhysicsSystem,Node)
        protected:
            static void _bind_methods();
        
        
        public:

            void _ready();
            void _process(double delta);
            
            //Export
            godot::Ref<PSS::ParticleBuffer> get_particle_buffer() const;
            void set_particle_buffer(godot::Ref<PSS::ParticleBuffer> buffer);

    };
} 