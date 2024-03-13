#pragma once


#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>



#include "core/elemental_spell_system.h"
#include "rendering/compute_shader.h"





namespace PSS
{

    enum Buffering
    {
        NONE = 0,
        DUAL = 1,
        TRIPLE = 2
    };


    class RenderingType
    {  
        public:
            static const int DEBUG = 0;
            static const int NONE = 1;
    };


    class ParticleRenderer : public godot::MeshInstance3D
    {
        GDCLASS(ParticleRenderer, godot::MeshInstance3D)

        
        private:
          

            godot::Ref<godot::Shader> m_overlayShader;
            godot::Ref<godot::RDShaderFile> m_renderingShaderResource;


            godot::Ref<ParticleBuffer> m_particleBuffer;
            std::array<ComputeShader,3> m_renderingShaders;
    

            godot::Viewport* m_viewport;



            bool m_compatibilityRendering;
            
       
            godot::Ref<godot::ImageTexture> m_texture;
            godot::Ref<godot::Image> m_image;

            //godot::String m_warning_string;



            void initializeRenderingShader();
            void updateRenderingShader();

    
            unsigned short m_currentFrame;
                    


        protected:

            void render_debug();
   
            godot::Camera3D * get_editor_camera();

            static void _bind_methods();
            //godot::PackedStringArray _get_configuration_warnings();
        public:
      
            
            //Export
            void set_compatibility_rendering(bool type);
            bool get_compatibility_rendering() const;


            //Export
            void set_overlay_shader(godot::Ref<godot::Shader> shader);
            godot::Ref<godot::Shader> get_overlay_shader() const;

            void set_rendering_shader(godot::Ref<godot::RDShaderFile> shader);
            godot::Ref<godot::RDShaderFile> get_rendering_shader() const;


            size_t get_rendered_particles() const;
            size_t get_total_particles() const;



            void _ready();
            void _process(double p_delta);
                        
            void render();


            ParticleRenderer();
        };
} 
