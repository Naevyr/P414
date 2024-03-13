#include "core/particle_data_container.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <array>
using namespace PSS;

bool sphere_intersection(const godot::Vector3& camera, const godot::Vector3& current_particle, const godot::Vector3& particle, float size)  {
    godot::Vector3 oc = camera - particle;
	float b = oc.dot( current_particle );
	float c = oc.dot(oc) - size * size;
	float h = b*b - c;
	if( h<0.0 ) return false;
	return true;
}

Particle PSS::ParticleDataContainer::add_particle(const godot::Ref<ElementalParticleResource> particleTemplate, godot::Vector3 position)
{
    Particle particle = m_particles.size();
    m_particles.push_back(particle);

    m_colors.push_back(particleTemplate->get_color());
    m_positions.push_back(position);

    return particle;
}

godot::Ref<godot::BoxMesh> PSS::ParticleDataContainer::get_bounds() const
{
    return m_bounds;
}

void PSS::ParticleDataContainer::set_bounds(godot::Ref<godot::BoxMesh> bounds)
{
    m_bounds = bounds;
}

size_t PSS::ParticleDataContainer::get_particle_count() const
{
    return m_particles.size();
}
size_t PSS::ParticleDataContainer::get_culled_particle_count() const
{
    return m_culledParticles.size();
}



godot::Color PSS::ParticleDataContainer::get_particle_color(Particle particle) const
{
    return m_colors[particle];
}

void PSS::ParticleDataContainer::set_particle_color(Particle particle, godot::Color color)
{
    m_colors[particle] = color;
}

godot::Vector3 PSS::ParticleDataContainer::get_position(Particle particle) const
{
    return m_positions[particle];
}

void PSS::ParticleDataContainer::set_position(Particle particle, godot::Vector3 position)
{
    m_positions[particle] = position;
}

float PSS::ParticleDataContainer::get_particle_size() const
{
    return m_size;
}

void PSS::ParticleDataContainer::set_particle_size( float size)
{
    m_size = size;
}


const std::vector<Particle> &PSS::ParticleDataContainer::get_particles() const 
{
    if (m_culledParticles.size() > 0)
        return m_culledParticles;
    
    return m_particles;
}
void PSS::ParticleDataContainer::cull_particles(godot::Vector3 cameraPosition) 
{

    std::vector<float> distances(m_particles.size());
    m_culledParticles.clear();


    for (auto &&particle : m_particles)
    {
        distances[particle] = m_positions[particle].distance_to(cameraPosition);
    }
    



    

    std::sort(m_particles.begin(), m_particles.end(), [&](Particle i, Particle j) {return  distances[i] < distances[j]; });

 

    for (size_t i = 0; i < m_particles.size(); i++)
    {
        auto particle = m_particles[i];
        
        bool collision = false;
        for (size_t j = 0; j < m_culledParticles.size() && !collision; j++)
        {
            auto culled_particle = m_culledParticles[j];
            
            auto proj = (m_positions[culled_particle] - cameraPosition).dot(m_positions[particle] - cameraPosition);
            proj = abs(proj / distances[particle]);
            auto lenght = distances[culled_particle];

            auto height = lenght * lenght - proj * proj;
            auto size = m_size * m_size;

         
            
            if (height <  size)
            {
                collision = true;
             
            }    
            
        }


        if(!collision)
            m_culledParticles.push_back(particle);
    }



}




void PSS::ParticleDataContainer::_bind_methods()
{
   
    godot::ClassDB::bind_method(godot::D_METHOD("get_bounds"), &PSS::ParticleDataContainer::get_bounds);
    godot::ClassDB::bind_method(godot::D_METHOD("set_bounds"), &PSS::ParticleDataContainer::set_bounds);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "bounds",
       godot::PROPERTY_HINT_RESOURCE_TYPE, "BoxMesh"), "set_bounds", "get_bounds");

   godot::ClassDB::bind_method(godot::D_METHOD("get_particle_size"), &PSS::ParticleDataContainer::get_particle_size);
    godot::ClassDB::bind_method(godot::D_METHOD("set_particle_size"), &PSS::ParticleDataContainer::set_particle_size);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "Particle Size"), "set_particle_size", "get_particle_size");

}


PSS::ParticleDataContainer::ParticleDataContainer()
{
    m_particles = std::vector<Particle>();
    m_culledParticles = std::vector<Particle>();
    m_colors = godot::PackedColorArray();
    m_positions = godot::PackedVector3Array();


}



