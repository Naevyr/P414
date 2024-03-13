#include "core/particle_buffer.h"

#include <stdexcept>
#include "particle_buffer.h"

using namespace PSS;
using namespace godot;


Particle ParticleBuffer::add_particle(const Ref<ElementalParticleResource> particleTemplate, const Vector3 position)
{
    return m_dataContainer->add_particle(particleTemplate,position);
}
void ParticleBuffer::add_cluster(const Ref<ElementalParticleResource> particleTemplate, const PackedVector3Array& positions)
{

   
    for (size_t i = 0; i < positions.size(); i++)
    {
        Particle particle = add_particle(particleTemplate,positions[i]);
      
    }


}

void ParticleBuffer::add_mesh(const Ref<ElementalMeshResource> mesh)
{
    add_cluster(mesh->get_template_particle(),mesh->get_position_array());
}

godot::Ref<ParticleDataContainer> ParticleBuffer::get_particle_data_container() 

{
    return m_dataContainer;
}



void ParticleBuffer::set_particle_data_container(godot::Ref<ParticleDataContainer> dataContainer)
{
    m_dataContainer = dataContainer;
}


unsigned int ParticleBuffer::get_particle_count() const
{
    return m_dataContainer->get_particle_count();
}


void ParticleBuffer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_particle_count"),&ParticleBuffer::get_particle_count);

    ClassDB::bind_method(D_METHOD("set_particle_data_container"),&ParticleBuffer::set_particle_data_container);
    ClassDB::bind_method(D_METHOD("get_particle_data_container"),&ParticleBuffer::get_particle_data_container);    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "data_container",
        PROPERTY_HINT_RESOURCE_TYPE, "ParticleDataContainer"), "set_particle_data_container", "get_particle_data_container");

}
      

ParticleBuffer::ParticleBuffer() 
{

}