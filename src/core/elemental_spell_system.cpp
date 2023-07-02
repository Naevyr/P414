#include "core/elemental_spell_system.h"

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/engine.hpp>
using namespace PSS;
using namespace godot;
void ElementalSpellSystem::_bind_methods() 
{

    ClassDB::bind_method(D_METHOD("set_particle_buffer"),&ElementalSpellSystem::set_particle_buffer);
    ClassDB::bind_method(D_METHOD("get_particle_buffer"),&ElementalSpellSystem::get_particle_buffer);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "particle_buffer",
        PROPERTY_HINT_RESOURCE_TYPE, "ParticleBuffer"), "set_particle_buffer", "get_particle_buffer");
    


}



void ElementalSpellSystem::set_particle_buffer(Ref<ParticleBuffer> particleBuffer)
{
    m_particleBuffer = particleBuffer;
}

Ref<ParticleBuffer> ElementalSpellSystem::get_particle_buffer() const
{
    return m_particleBuffer;
}


ElementalSpellSystem::ElementalSpellSystem()
{


}



