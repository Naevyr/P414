#pragma once

//Contains all the data needed to fill a new particle
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/color.hpp>

namespace PSS
{
    class ElementalParticleResource : public godot::Resource
    {
        GDCLASS(ElementalParticleResource,godot::Resource)

        private:
            float m_size;
            godot::Color m_color;

        protected:
           static void _bind_methods();
        
        public:

        
            void set_color(godot::Color color);
            godot::Color get_color() const;
        
            ElementalParticleResource();
    };

        
} // namespace PSS
