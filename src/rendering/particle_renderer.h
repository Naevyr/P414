#pragma once


#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "core/particle_system.h"
#include "core/compute_shader.h"
using namespace godot;




namespace PSS
{
    static class RenderingType
    {  
        public:
            static const int DEBUG = 0;
            static const int NONE = 1;
    };


    class ParticleRenderer : public MeshInstance3D
    {
        GDCLASS(ParticleRenderer, MeshInstance3D)

        
        private:
            const string OVERLAY_SHADER = "res://bin/overlayshader.gdshader";
            const string RENDERING_SHADER = "res://bin/shader.glsl";



            Vector2i m_viewportSize = Vector2i(0,0);
            int m_rendering_type = 0;
            
            
            std::map<int,ComputeShader> m_shaders;
            Ref<ImageTexture> m_texture;
        

            //godot::String m_warning_string;


            
                    


        protected:

            void render_debug();
            void get_data(int p_rendering_type, int p_binding, PackedByteArray& r_buffer);
            Camera3D * get_editor_camera();

            static void _bind_methods();
            //godot::PackedStringArray _get_configuration_warnings();
        public:
      
            
            //TODO: Temporary enum property fix for GDExtension 
            inline void set_rendering_type(int p_type) {  m_rendering_type = p_type; };
            inline int get_rendering_type() const { return  m_rendering_type; }

            void _ready();
            void _process(double p_delta);
                        
            void render();


            ParticleRenderer();
        };
} // namespace P414.Rendering
