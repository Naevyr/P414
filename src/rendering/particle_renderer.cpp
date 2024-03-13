#include "rendering/particle_renderer.h"


#include "rendering/particle_gpu_encoder.h"

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/editor_script.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/quad_mesh.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/viewport_texture.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <exception>



using namespace PSS;
using namespace godot;




void ParticleRenderer::_ready()
{

    Node::_ready();



    if (Engine::get_singleton()->is_editor_hint())
        return;
    
    
    auto particleSystem = Node::cast_to<ElementalSpellSystem>(get_parent());

    if (!particleSystem)
    {
        ERR_PRINT("ParticleRenderer is not a child of a ElementalSpellSystem node.");
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);


        return;
    }

    m_particleBuffer = particleSystem->get_particle_buffer();


    auto mesh = Ref<QuadMesh>();
    mesh.instantiate();


    mesh->set_size(Vector2(2,2));
    this->set_mesh(mesh);
    auto material = Ref<ShaderMaterial>();
    material.instantiate();

    material->set_shader(m_overlayShader);
    this->set_material_override(material);
    
    this->set_extra_cull_margin(1e20);

    m_viewport = get_viewport();
    
    
    
    
    if (!m_viewport->get_camera_3d())
    {
        ERR_PRINT("Scene does not contain a Camera3D node.");
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }   
    
    
    
    
    Vector2 size = m_viewport->get_texture()->get_size();

    m_image = Image::create(size.x,size.y,false,Image::FORMAT_RGBAF);
    m_texture = ImageTexture::create_from_image(m_image);
    material->set_shader_parameter("overlay",m_texture);
    m_renderingShaders = {ComputeShader(m_renderingShaderResource),ComputeShader(m_renderingShaderResource),ComputeShader(m_renderingShaderResource)};


    

    initializeRenderingShader();

  
    


    m_currentFrame = 0;
  
}

void ParticleRenderer::_process(double p_delta)
{
    Node::_process(p_delta);
    if(Engine::get_singleton()->is_editor_hint())
        return;




    if(m_particleBuffer->get_particle_count() == 0)
        return;

  

    



    
    
    Vector2 size = m_viewport->get_texture()->get_size();
    auto data = m_renderingShaders[m_currentFrame].get_uniform_data(0);
    m_texture->update(Image::create_from_data(size.x,size.y,false,Image::FORMAT_RGBAF,data));


    updateRenderingShader();
    m_renderingShaders[m_currentFrame].dispatch(Vector3i(size.x,size.y ,1));
    

    m_currentFrame = (m_currentFrame + 1) % 3;
}


void ParticleRenderer::initializeRenderingShader()
{
    for (size_t i = 0; i < 3; i++)
    {
        m_renderingShaders[i].add_uniform(0,UniformType::Image, m_image);
        m_renderingShaders[i].add_uniform(1,UniformType::Buffer, ParticleGPUEncoder::encode_particle_ordered(m_particleBuffer,m_viewport->get_camera_3d()->get_global_position()));
      
        m_renderingShaders[i].add_uniform(2,UniformType::Buffer, ParticleGPUEncoder::encode_particle_positions(m_particleBuffer));
        m_renderingShaders[i].add_uniform(3,UniformType::Buffer, ParticleGPUEncoder::encode_particle_colors(m_particleBuffer));
        m_renderingShaders[i].add_uniform(4,UniformType::Buffer, ParticleGPUEncoder::encode_camera(m_viewport->get_camera_3d()));
        
        m_renderingShaders[i].compile_shader();

    }
    
}
void ParticleRenderer::updateRenderingShader()
{
    /*
    
    m_renderingComputeShader.update_uniform(1,ParticleGPUEncoder::encode_particle_ordered(m_particleBuffer,m_viewport->get_camera_3d()->get_global_position()));   
    m_renderingComputeShader.update_uniform(2, ParticleGPUEncoder::encode_particle_positions(m_particleBuffer));
    m_renderingComputeShader.update_uniform(3,ParticleGPUEncoder::encode_particle_colors(m_particleBuffer));
    m_renderingComputeShader.update_uniform(4,ParticleGPUEncoder::encode_camera(m_viewport->get_camera_3d()));
    
    */

    
    m_renderingShaders[m_currentFrame].update_uniform(1,ParticleGPUEncoder::encode_particle_ordered(m_particleBuffer,m_viewport->get_camera_3d()->get_global_position()));   
    //m_renderingComputeShader.update_uniform(2,ParticleGPUEncoder::encode_octree_octans(m_particleBuffer));   
    m_renderingShaders[m_currentFrame].update_uniform(2,ParticleGPUEncoder::encode_particle_positions(m_particleBuffer));
    m_renderingShaders[m_currentFrame].update_uniform(3,ParticleGPUEncoder::encode_particle_colors(m_particleBuffer));
    m_renderingShaders[m_currentFrame].update_uniform(4,ParticleGPUEncoder::encode_camera(m_viewport->get_camera_3d()));
    
    
}






/*
PackedStringArray ParticleRenderer::_get_configuration_warnings()
{
    PackedStringArray warnings = Node::_get_configuration_warnings();
    
    if(!Object::cast_to<ElementalSpellSystem>(get_parent()))
        warnings.push_back("Parent is not a ElementalSpellSystem, this node won't be able to render the particles on your scene");
    
    return warnings;
}

*/
void ParticleRenderer::_bind_methods()
{
    //ClassDB::bind_method(D_METHOD("_get_configuration_warnings"),&ParticleRenderer::_get_configuration_warnings);
    ClassDB::bind_method(D_METHOD("set_compatibility_rendering"),&ParticleRenderer::set_compatibility_rendering);
    ClassDB::bind_method(D_METHOD("get_compatibility_rendering"),&ParticleRenderer::get_compatibility_rendering);
    
    ClassDB::bind_method(D_METHOD("set_rendering_shader"),&ParticleRenderer::set_rendering_shader);
    ClassDB::bind_method(D_METHOD("get_rendering_shader"),&ParticleRenderer::get_rendering_shader);



    ClassDB::bind_method(D_METHOD("set_overlay_shader"),&ParticleRenderer::set_overlay_shader);
    ClassDB::bind_method(D_METHOD("get_overlay_shader"),&ParticleRenderer::get_overlay_shader);
    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::BOOL, "compatibility_rendering"), "set_compatibility_rendering", "get_compatibility_rendering");

    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "rendering_shader",
        PROPERTY_HINT_RESOURCE_TYPE, "RDShaderFile"), "set_rendering_shader", "get_rendering_shader");
    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "overlay_shader",
        PROPERTY_HINT_RESOURCE_TYPE, "Shader"), "set_overlay_shader", "get_overlay_shader");


    ClassDB::bind_method(D_METHOD("get_particle_count"),&ParticleRenderer::get_total_particles);
    ClassDB::bind_method(D_METHOD("get_rendered_particle_count"),&ParticleRenderer::get_rendered_particles);

   // ClassDB::add_property("ParticleRenderer",PropertyInfo(Variant::INT, "rendering_type",PROPERTY_HINT_ENUM,"DEBUG,NONE"), "set_rendering_type", "get_rendering_type");
    //ClassDB::add_property("ParticleRenderer",PropertyInfo(Variant::INT, "rendering_type"), "set_rendering_type", "get_rendering_type");

}


//Export
void ParticleRenderer::set_compatibility_rendering(bool compatibility_rendering)
{
    m_compatibilityRendering = compatibility_rendering;
}
bool ParticleRenderer::get_compatibility_rendering() const
{
    return m_compatibilityRendering;
}


//Export
void ParticleRenderer::set_overlay_shader(Ref<Shader> shader)
{
    m_overlayShader = shader;
}
Ref<Shader> ParticleRenderer::get_overlay_shader() const
{
    return m_overlayShader;
}

void ParticleRenderer::set_rendering_shader(Ref<RDShaderFile> shader)
{
    m_renderingShaderResource = shader;
}
Ref<RDShaderFile> ParticleRenderer::get_rendering_shader() const
{
    return m_renderingShaderResource;
}

size_t PSS::ParticleRenderer::get_rendered_particles() const
{
    return m_particleBuffer->get_particle_data_container()->get_culled_particle_count();
}


size_t PSS::ParticleRenderer::get_total_particles() const
{
    return m_particleBuffer->get_particle_count();
}

ParticleRenderer::ParticleRenderer()
{
   
    
}