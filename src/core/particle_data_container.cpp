#include "core/particle_data_container.h"

using namespace PSS;
using namespace godot;

Particle ParticleDataContainer::add_particle(const Ref<ElementalParticleResource> p_particleTemplate, Vector3 p_position)
{


    Particle particle;
    if(m_deletedParticles.size() == 0)
    { 
        particle = m_positions.size();
        m_positions.push_back(Vector3());
        m_colors.push_back(Color());
        m_sizes.push_back(0);
    }else
    {
        particle = m_deletedParticles.back();
        m_deletedParticles.pop();
    }
    

    m_positions[particle] = p_position;
    m_colors[particle] = p_particleTemplate->get_color();
    m_sizes[particle] = p_particleTemplate->get_size();
    m_particles.push_back(particle);

    return particle;
}


 void ParticleDataContainer::_bind_methods()
 {

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



const std::vector<Particle>& ParticleDataContainer::get_particles() const
{
    return m_particles;
}


ParticleDataContainer::ParticleDataContainer()
{
    m_positions = PackedVector3Array();
    m_colors = PackedColorArray();
    m_sizes = PackedFloat32Array();
}

