#pragma once


#include <map>
#include <memory>
#include <string>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/classes/rendering_device.hpp>




namespace PSS
{
    enum UniformType
    {
        Image,
        Buffer
    };

    struct ShaderUniform
    { 
        UniformType type;
        RID buffer;
        RDUniform uniform = RDUniform();
        ShaderUniform operator=(ShaderUniform uniform) {return uniform;}
        ShaderUniform() {}
    }; 


    class ComputeShader
    {
        private:
            
            std::map<int32_t,ShaderUniform> m_uniforms;
            RID m_shader;
            RID m_uniform_set;
            RID m_pipeline;
            TypedArray<RDUniform> m_uniforms_data;
            RenderingDevice * m_renderingDevice;
        
         
            
       
        public:


        void add_uniform(int32_t p_binding, UniformType p_type, godot::Ref<godot::Image> p_image );
        void add_uniform(int32_t p_binding, UniformType p_type, const PackedByteArray& p_data);
        void add_uniform(int32_t p_binding,UniformType p_type,  RID p_bufferRID);
        void update_uniform(int32_t p_binding,const PackedByteArray& p_data);
        void compile_shader();
		void dispatch(Vector3i p_workgroups);

        godot::PackedByteArray get_uniform_data(int32_t p_binding);
        
        ComputeShader();
        ComputeShader(std::string p_path, RenderingDevice * p_renderingDevice);
        
    };

}