#include "core/particle_data_container.h"

using namespace PSS;
using namespace godot;

Particle ParticleDataContainer::add_particle(const Ref<ElementalParticleResource> p_particleTemplate, Vector3 p_position)
{
    m_positions.append(p_position);
    m_colors.append(p_particleTemplate->get_color());

}







        


Color ParticleDataContainer::get_particle_color(Particle particle) const
{
    return m_colors[particle];
}
void ParticleDataContainer::set_particle_color(Particle particle, Color color)
{
    m_colors[particle] = color;
}




Vector3 ParticleDataContainer::get_position(Particle particle) const
{
    return m_positions[particle];
}


void ParticleDataContainer::set_position(Particle particle, Vector3 position)
{
    m_positions[particle] = position;
}


float ParticleDataContainer::get_size(Particle particle) const
{
    return m_sizes[particle];
}



void ParticleDataContainer::set_size(Particle particle, float size)
{
    m_sizes[particle] = size;
}





ParticleDataContainer::ParticleDataContainer()
{
    m_positions = PackedVector3Array();
    m_colors = PackedColorArray();
    m_sizes = PackedFloat32Array();
}

