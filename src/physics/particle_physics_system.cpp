#include <godot_cpp/classes/engine.hpp>


#include "physics/particle_physics_system.h"
#include "core/elemental_spell_system.h"
#include <chrono>


using namespace PSS;
using namespace godot;



void ParticlePhysicsSystem::_bind_methods()
{

    ClassDB::bind_method(D_METHOD("set_particle_buffer"),&ParticlePhysicsSystem::set_particle_buffer);
    ClassDB::bind_method(D_METHOD("get_particle_buffer"),&ParticlePhysicsSystem::get_particle_buffer);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "particle_buffer",
        PROPERTY_HINT_RESOURCE_TYPE, "ParticleBuffer"), "set_particle_buffer", "get_particle_buffer");
}

void ParticlePhysicsSystem::_ready()
{
    Node::_ready();  
    
    
    ElementalSpellSystem * system = Node::cast_to<ElementalSpellSystem>(get_parent());
    if (Engine::get_singleton()->is_editor_hint())
    {
        if(!system)
            throw std::runtime_error("ParticlePhysicsSystem is not a child of a ElementalSpellSystem node.");
    
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }
    


   

    m_buffer = system->get_particle_buffer();


}


void ParticlePhysicsSystem::_process(double delta)
{
    Node::_process(delta);


}


void ParticlePhysicsSystem::set_particle_buffer(Ref<ParticleBuffer> buffer) 
{
    m_buffer = buffer;
}

Ref<ParticleBuffer> ParticlePhysicsSystem::get_particle_buffer() const
{
    return m_buffer;
}