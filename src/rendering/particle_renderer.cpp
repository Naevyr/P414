#include "particle_renderer.h"

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



using namespace PSS;


void ParticleRenderer::_ready()
{

    Node::_ready();
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    
    auto particleSystem = Node::cast_to<ElementalSpellSystem>(get_parent());
 
    if(!particleSystem)    
        throw std::exception("ParticleRenderer is not a child of a ElementalSpellSystem node.");



    auto mesh = Ref<QuadMesh>();
    mesh.instantiate();


    mesh->set_size(Vector2(2,2));
    this->set_mesh(mesh);
    auto material = Ref<ShaderMaterial>();
    material.instantiate();
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(godot::String(OVERLAY_SHADER.c_str()));
    material->set_shader(shader);
    this->set_material_override(material);
    
    this->set_extra_cull_margin(1e20);
    m_viewportSize = get_editor_camera()->get_viewport()->get_texture()->get_size();
    auto image = Image::create(m_viewportSize.x,m_viewportSize.y,false,Image::FORMAT_RGBAF);
    m_texture = ImageTexture::create_from_image(image);
    material->set_shader_parameter("overlay",m_texture);


    m_shaders[m_rendering_type] = ComputeShader(RENDERING_SHADER, particleSystem->get_rendering_device());
    
    
    
    
    m_shaders[m_rendering_type].add_uniform(0,UniformType::Image,image);
    m_shaders[m_rendering_type].add_uniform(1,UniformType::Buffer,particleSystem->get_particle_buffer()->get_GPU_reference());
    PackedByteArray cameraBuffer;
    get_data(m_rendering_type,2,cameraBuffer);
    m_shaders[m_rendering_type].add_uniform(2,UniformType::Buffer,cameraBuffer);

    
    m_shaders[m_rendering_type].compile_shader();

    
  
}

void ParticleRenderer::_process(double p_delta)
{
    Node::_process(p_delta);

    PackedByteArray cameraBuffer;
    get_data(m_rendering_type,2,cameraBuffer);
    m_shaders[m_rendering_type].update_uniform(2,cameraBuffer);

    m_shaders[m_rendering_type].dispatch(Vector3i(m_viewportSize.x,m_viewportSize.y,1));
    auto data = m_shaders[m_rendering_type].get_uniform_data(0);
    
    m_texture->update(Image::create_from_data(m_viewportSize.x,m_viewportSize.y,false,Image::FORMAT_RGBAF,data));
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
    ClassDB::bind_method(D_METHOD("set_rendering_type"),&ParticleRenderer::set_rendering_type);
    ClassDB::bind_method(D_METHOD("get_rendering_type"),&ParticleRenderer::get_rendering_type);
    
   // ClassDB::add_property("ParticleRenderer",PropertyInfo(Variant::INT, "rendering_type",PROPERTY_HINT_ENUM,"DEBUG,NONE"), "set_rendering_type", "get_rendering_type");
    //ClassDB::add_property("ParticleRenderer",PropertyInfo(Variant::INT, "rendering_type"), "set_rendering_type", "get_rendering_type");

}



void ParticleRenderer::get_data(int p_type, int p_binding, PackedByteArray& r_buffer)
{
    
    switch (p_type)
    {
        case RenderingType::DEBUG:
        
        {
            switch (p_binding)
            {
                /*
                case 1:
                {


                    ElementalSpellSystem * parent = (ElementalSpellSystem*)get_parent();
                    if(parent != nullptr)
                    {
                        
                        r_buffer.append_array(parent->get_particle_buffer().get_data());    

                       

                    }


                }

                break;
                */

               
                case 2:
                {
                    Camera3D * camera = get_editor_camera();
                  

                    r_buffer.resize(68);
                  
                    Transform3D transform = camera->get_transform();
                    //Vector3 globalForward = camera->get_global_transform().xform(localForward);
                    
                    




                    int ind = 0;


                    for (size_t i = 0; i < 3; i++)
                    {
                        for (size_t j = 0; j < 3; j++)
                        {
                            r_buffer.encode_float(ind,transform.basis.rows[i][j]);
                            ind += 4;
                        }
                        ind += 4;
                    }
                    
                    r_buffer.encode_float(ind,transform.origin.x);
                    r_buffer.encode_float(ind + 4,transform.origin.y);
                    r_buffer.encode_float(ind + 8,transform.origin.z);


                    
               
                    float fov = camera->get_fov();
                    r_buffer.encode_float(64,fov);

                    UtilityFunctions::print(fov);
                    
                    

                }
                break;


                default:
                    break;
            }

                
        }

        break;
    
    default:
        break;
    }

}

std::vector<Camera3D*> step(Node * node)
{
    auto camera = Node::cast_to<Camera3D>(node);

    std::vector<Camera3D*> array;
    for (size_t i = 0; i < node->get_child_count(); i++)
    {
        auto res = step(node->get_child(i));
        for (auto &&e : res)
        {
            array.push_back(e);
        }
        
    }

    if(camera != nullptr)
        array.push_back(camera);
    return array; 
   
    
}

Camera3D * ParticleRenderer::get_editor_camera()
{
    auto interface = EditorScript().get_editor_interface()->get_editor_main_screen();
    auto cameras = step(VBoxContainer::cast_to<Node>(interface));
    auto editorCamera = cameras[0];
    return (Camera3D*)editorCamera;
}




ParticleRenderer::ParticleRenderer()
{
   
    
}