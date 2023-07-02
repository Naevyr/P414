#include "rendering/compute_shader.h"


#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>


using namespace PSS;
using namespace godot;



void ComputeShader::add_uniform(int32_t p_binding ,UniformType p_type, Ref<godot::Image> p_image)
{

    m_uniforms.insert_or_assign(p_binding, ShaderUniform());

    m_uniforms[p_binding].type = p_type;

    m_uniforms[p_binding].uniform.set_uniform_type(RenderingDevice::UniformType::UNIFORM_TYPE_IMAGE);
    m_uniforms[p_binding].uniform.set_binding(p_binding);
    
    auto format = new RDTextureFormat();
    
    format->set_format(RenderingDevice::DataFormat::DATA_FORMAT_R32G32B32A32_SFLOAT);
    format->set_width(p_image->get_width());
    format->set_height(p_image->get_height());
    format->set_usage_bits(
        godot::RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT | 
        godot::RenderingDevice::TEXTURE_USAGE_STORAGE_BIT |
        godot::RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT 
        );  

    
    RID texture = m_renderingDevice->texture_create(format, new RDTextureView());
    m_uniforms[p_binding].uniform.add_id(texture);

    m_uniforms[p_binding].buffer = texture;
    




}
void ComputeShader::add_uniform(int32_t p_binding ,UniformType p_type,const PackedByteArray& p_data)
{
    
    m_uniforms.insert_or_assign(p_binding, ShaderUniform());

    m_uniforms[p_binding].type = p_type;

    
    
    m_uniforms[p_binding].uniform.set_uniform_type(RenderingDevice::UniformType::UNIFORM_TYPE_STORAGE_BUFFER);
    m_uniforms[p_binding].uniform.set_binding(p_binding);
    auto size = p_data.size();
    auto buffer = m_renderingDevice->storage_buffer_create(size,p_data);
    m_uniforms[p_binding].uniform.add_id(buffer);

    m_uniforms[p_binding].buffer = buffer;
    
    



    
}
void ComputeShader::add_uniform(int32_t p_binding,UniformType p_type, RID p_bufferRID)
{
       
    m_uniforms.insert_or_assign(p_binding, ShaderUniform());

    m_uniforms[p_binding].type = p_type;

    
    
    m_uniforms[p_binding].uniform.set_uniform_type(RenderingDevice::UniformType::UNIFORM_TYPE_STORAGE_BUFFER);
    m_uniforms[p_binding].uniform.set_binding(p_binding);

    m_uniforms[p_binding].uniform.add_id(p_bufferRID);

    m_uniforms[p_binding].buffer = p_bufferRID;
    
}

godot::PackedByteArray ComputeShader::get_uniform_data(int32_t p_binding)
{
        if(m_uniforms[p_binding].type == UniformType::Image)
            return m_renderingDevice->texture_get_data(m_uniforms[p_binding].buffer,0);
        else
            return m_renderingDevice->buffer_get_data(m_uniforms[p_binding].buffer);
}

void ComputeShader::update_uniform(int32_t p_binding,const PackedByteArray& p_data)
{
    if(m_uniforms[p_binding].type == UniformType::Image)
        m_renderingDevice->texture_update(m_uniforms[p_binding].buffer,0, p_data);
    else
        m_renderingDevice->buffer_update(m_uniforms[p_binding].buffer,0,p_data.size(), p_data);
}
void ComputeShader::compile_shader()
{   

    m_uniforms_data =  TypedArray<RDUniform>();

    for (auto &&uniform :  m_uniforms)
    {
        
        m_uniforms_data.append(&uniform.second.uniform);
        
    }
    
  
    m_pipeline = m_renderingDevice->compute_pipeline_create(m_shader);
   
 

}

void ComputeShader::dispatch(Vector3i p_workgroups)
{
    m_uniform_set = m_renderingDevice->uniform_set_create(m_uniforms_data, m_shader, 0);
    auto computeList = m_renderingDevice->compute_list_begin();


    m_renderingDevice->compute_list_bind_compute_pipeline(computeList, m_pipeline);
    m_renderingDevice->compute_list_bind_uniform_set(computeList, m_uniform_set, 0);
    m_renderingDevice->compute_list_dispatch(computeList, p_workgroups.x, p_workgroups.y, p_workgroups.z);

    m_renderingDevice->compute_list_end();
  

    m_renderingDevice->submit();
    m_renderingDevice->sync();
}


ComputeShader::ComputeShader(){}

ComputeShader::ComputeShader(const Ref<RDShaderFile> shader )
{ 
    m_renderingDevice = RenderingServer::get_singleton()->create_local_rendering_device();
    
    const auto spirV = shader->get_spirv();
    
    m_shader = m_renderingDevice->shader_create_from_spirv(spirV);



}
