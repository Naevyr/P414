#pragma once


#include <map>
#include <memory>
#include <string>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/typed_array.hpp>




namespace PSS
{
    enum UniformType
    {
        Image,
        Buffer
    };

    struct ShaderUniform
    { 
        public:
            UniformType type;
            godot::RID buffer;
            godot::RDUniform uniform = godot::RDUniform();
            ShaderUniform operator=(ShaderUniform uniform) {return uniform;}
            ShaderUniform() {}
    }; 


    class ComputeShader
    {
        private:
            
            std::map<int32_t,ShaderUniform> m_uniforms;
            std::map<int32_t,size_t> m_uniformSizes;
            godot::RID m_shader;
            godot::RID m_uniform_set;
            godot::RID m_pipeline;
            godot::TypedArray< godot::RDUniform> m_uniforms_data;
            godot::RenderingDevice * m_renderingDevice;
    
            bool m_initialized = false;
            
      
        public: 
            void add_uniform(int32_t p_binding, UniformType p_type, godot::Ref<godot::Image> p_image );
            void add_uniform(int32_t p_binding, UniformType p_type, const godot::PackedByteArray& p_data);
            void add_uniform(int32_t p_binding,UniformType p_type,  godot::RID p_bufferRID);
            void update_uniform(int32_t p_binding,const  godot::PackedByteArray& p_data);
            void compile_shader();
            void dispatch( godot::Vector3i p_workgroups);
            void wait_for_completion();
            godot::PackedByteArray get_uniform_data(int32_t p_binding);
            
            bool is_initalized() const;

            ComputeShader();
            ComputeShader(const godot::Ref<godot::RDShaderFile> shader );
        
    };

}