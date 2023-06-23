#include "core/elemental_spell_system.h"

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/engine.hpp>
using namespace PSS;
using namespace godot;
void ElementalSpellSystem::_bind_methods() 
{

}





void ElementalSpellSystem::_ready()
{
    Node::_ready();
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);


    //m_particleBuffer->init(get_rendering_device());
    //m_particleBuffer->sync();
}

void ElementalSpellSystem::_process(double delta)
{
  
    Node::_process(delta);
    
  
}
ElementalSpellSystem::ElementalSpellSystem()
{


   


    



}









Ref<ParticleBuffer> ElementalSpellSystem::get_particle_buffer() const
{
    return m_particleBuffer;
}

void ElementalSpellSystem::set_particle_buffer(Ref<ParticleBuffer> particleBuffer)
{
    m_particleBuffer = particleBuffer;
}