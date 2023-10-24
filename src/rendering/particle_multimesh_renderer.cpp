
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>



#include "rendering/particle_multimesh_renderer.h"
#include "core/elemental_spell_system.h"

using namespace PSS;
using namespace godot;

void ParticleMultimeshRenderer::_ready()
{
    
    if (Engine::get_singleton()->is_editor_hint())
    {
       
        return;
    }
    
    auto particleSystem = Node::cast_to<ElementalSpellSystem>(get_parent());

    if (!particleSystem)
    {
        ERR_PRINT("ParticleMultimeshRenderer is not a child of a ElementalSpellSystem node.");
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }

    m_particleBuffer = particleSystem->get_particle_buffer();


    
    
    if (!m_particleBuffer.is_valid())
    {
        ERR_PRINT("ParticleMultimeshRenderer is not a child of a ElementalSpellSystem node.");
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }

}



void ParticleMultimeshRenderer::_process(double delta)
{
    MultiMeshInstance3D::_process(delta);
    if(Engine::get_singleton()->is_editor_hint() || m_initialized || m_particleBuffer->get_particle_count() == 0)
        return;
    
    auto mesh = MultiMeshInstance3D::get_multimesh();
    if(!mesh.is_valid())
    {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }   

    mesh->set_instance_count(m_particleBuffer->get_particle_count());
    auto data = m_particleBuffer->get_data();
    for (auto &&particle : data.get_particles())
    {
        
        mesh->set_instance_transform(particle,Transform3D(Basis(),data.get_position(particle)));
    }
    


}
void ParticleMultimeshRenderer::_bind_methods() {}

ParticleMultimeshRenderer::ParticleMultimeshRenderer() {}