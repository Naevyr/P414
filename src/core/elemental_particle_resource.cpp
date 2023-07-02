#include "core/elemental_particle_resource.h"


using namespace PSS;
using namespace godot;

void ElementalParticleResource::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_size"),&ElementalParticleResource::set_size);
    ClassDB::bind_method(D_METHOD("get_size"),&ElementalParticleResource::get_size);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "size"), "set_size", "get_size");

    ClassDB::bind_method(D_METHOD("set_color"),&ElementalParticleResource::set_color);
    ClassDB::bind_method(D_METHOD("get_color"),&ElementalParticleResource::get_color);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::COLOR, "color"), "set_color", "get_color");
    
}


void ElementalParticleResource::set_size(float size) 
{
    m_size = size;
}
float ElementalParticleResource::get_size() const
{
    return m_size;
}
    
void ElementalParticleResource::set_color(Color color) 
{
    m_color =  color;
}
Color ElementalParticleResource::get_color() const 
{
    return m_color;
}
    
ElementalParticleResource::ElementalParticleResource()
{

}
;

        
