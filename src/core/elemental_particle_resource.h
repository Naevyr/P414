#pragma once

//Contains all the data needed to fill a new particle
#include <godot_cpp/classes/resource.hpp>

namespace PSS
{
    class ElementalParticleResource : public godot::Resource
    {
        GDCLASS(ElementalParticleResource,Resource)

        private:
            float m_size;
            godot::Vector3 m_color;

        protected:
           static void _bind_methods();
        
        public:

            float get_size() const;
            void set_size(float p_size) const;
        
            godot::Color get_color() const;
            void set_color(godot::Color p_color) const;
        
        ElementalParticleResource();
    };

        
} // namespace PSS
