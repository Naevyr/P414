#include <godot_cpp/classes/engine.hpp>


#include "physics/particle_physics_system.h"
#include "core/elemental_spell_system.h"
#include <chrono>


using namespace PSS;
using namespace godot;


void ParticlePhysicsSystem::_ready()
{
    Node::_ready();

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);


    ElementalSpellSystem * system = Node::cast_to<ElementalSpellSystem>(get_parent());


    if(!system)
        throw std::exception("ParticlePhysicsSystem is not a child of a ElementalSpellSystem node.");
    

   

    m_buffer = system->get_particle_buffer();


}


void ParticlePhysicsSystem::_process(double delta)
{
    Node::_process(delta);


}


