#include "core/elemental_spell_system.h"

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
using namespace PSS;
using namespace godot;
void ElementalSpellSystem::_bind_methods() 
{

    ClassDB::bind_method(D_METHOD("set_particle_buffer"),&ElementalSpellSystem::set_particle_buffer);
    ClassDB::bind_method(D_METHOD("get_particle_buffer"),&ElementalSpellSystem::get_particle_buffer);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "particle_buffer",
        PROPERTY_HINT_RESOURCE_TYPE, "ParticleBuffer"), "set_particle_buffer", "get_particle_buffer");
    

    ClassDB::bind_method(D_METHOD("set_debug_mesh_resource"),&ElementalSpellSystem::set_debug_mesh_resource);
    ClassDB::bind_method(D_METHOD("get_debug_mesh_resource"),&ElementalSpellSystem::get_debug_mesh_resource);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "debug_mesh_resource",
        PROPERTY_HINT_RESOURCE_TYPE, "ElementalMeshResource"), "set_debug_mesh_resource", "get_debug_mesh_resource");
}



void ElementalSpellSystem::set_particle_buffer(Ref<ParticleBuffer> particleBuffer)
{
    m_particleBuffer = particleBuffer;
}

Ref<ParticleBuffer> ElementalSpellSystem::get_particle_buffer() const
{
    return m_particleBuffer;
}

void ElementalSpellSystem::set_debug_mesh_resource(godot::Ref<ElementalMeshResource> meshResource)
{
    m_debugMeshResource = meshResource;
}
godot::Ref<ElementalMeshResource> ElementalSpellSystem::get_debug_mesh_resource() const
{
    return m_debugMeshResource;
}
        

void ElementalSpellSystem::_ready()
{

    Node::_ready();
    if(Engine::get_singleton()->is_editor_hint())
    {
        if(m_debugMeshResource.is_valid())
        {
            float size = m_particleBuffer->get_particle_data_container()->get_particle_size();
            m_debugMeshResource->generate_mesh(size);
        }

        return;
    }


    if(m_debugMeshResource.is_valid() && m_particleBuffer.is_valid())
        m_particleBuffer->add_mesh(m_debugMeshResource);
    
    for (auto &&particle : m_particleBuffer->get_particle_data_container()->get_particles())
    {
        m_particleBuffer->get_particle_data_container()->set_particle_color(particle,Color::from_hsv((godot::UtilityFunctions::randi() % 24) / 24.0, 1, 1));
    }
    
    
}
ElementalSpellSystem::ElementalSpellSystem()
{


}



