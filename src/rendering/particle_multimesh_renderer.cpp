
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "rendering/particle_multimesh_renderer.h"
#include "core/elemental_spell_system.h"

using namespace PSS;
using namespace godot;

void ParticleMultimeshRenderer::_ready()
{
    godot::MultiMeshInstance3D::_ready();

    
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

    m_viewport = get_viewport();
    
    
    
    
    if (!m_viewport->get_camera_3d())
    {
        ERR_PRINT("Scene does not contain a Camera3D node.");
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }   
    
    
    auto mesh = godot::MultiMesh::cast_to<godot::BoxMesh>(godot::MultiMeshInstance3D::get_multimesh()->get_mesh().ptr());
    float size = m_particleBuffer->get_particle_data_container()->get_particle_size();
    mesh->set_size(Vector3(size,size,size));
   // godot::UtilityFunctions::print(mesh);    

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
    auto data = m_particleBuffer->get_particle_data_container();
    data->cull_particles(m_viewport->get_camera_3d()->get_global_position());
    for (auto &&particle : data->get_particles())
    {
        
        mesh->set_instance_transform(particle,Transform3D(Basis(),data->get_position(particle)));
    }
    


}
void ParticleMultimeshRenderer::_bind_methods() {}

ParticleMultimeshRenderer::ParticleMultimeshRenderer() {}