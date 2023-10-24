#include "core/particle_buffer.h"

#include <stdexcept>

using namespace PSS;
using namespace godot;


Particle ParticleBuffer::add_particle(const Ref<ElementalParticleResource> particleTemplate, const Vector3 position)
{
    if(!m_octree.is_initialized())
        m_octree = LinearOctree(Vector3(0,0,0),m_bounds->get_size()[m_bounds->get_size().max_axis_index()],m_octanParticleCount,m_octanMaxDepth);


    Particle particle = m_data.add_particle(particleTemplate,position);
    m_octree.insert(particle,position,particleTemplate->get_size());
    m_particleCount += 1;


    return particle;
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



unsigned int ParticleBuffer::get_particle_count() const
{
    return m_particleCount;
}


std::vector<Particle> ParticleBuffer::get_nearby_particles(Vector3 position, float size) const
{
    return m_octree.query(position,size);
}



Ref<BoxMesh> ParticleBuffer::get_bounds() const
{
    return m_bounds;
}

void ParticleBuffer::set_bounds(Ref<BoxMesh> convexBounds)
{

    
    m_bounds = convexBounds;
}

ParticleDataContainer& ParticleBuffer::get_data()
{
    return m_data;
}

const LinearOctree & ParticleBuffer::get_octree() const
{
    return m_octree;
}

void ParticleBuffer::set_octan_particle_count(int octanParticleCount)
{
   m_octanParticleCount = octanParticleCount;
}
int ParticleBuffer::get_octan_particle_count() const
{
    return m_octanParticleCount;
}

void ParticleBuffer::set_octan_max_depth(int octanMaxDepth)
{
    m_octanMaxDepth = octanMaxDepth;
}
int ParticleBuffer::get_octan_max_depth() const
{
    return m_octanMaxDepth;
}




void ParticleBuffer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_particle_count"),&ParticleBuffer::get_particle_count);

    ClassDB::bind_method(D_METHOD("set_bounds"),&ParticleBuffer::set_bounds);
    ClassDB::bind_method(D_METHOD("get_bounds"),&ParticleBuffer::get_bounds);    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "bounds",
        PROPERTY_HINT_RESOURCE_TYPE, "BoxMesh"), "set_bounds", "get_bounds");
    

    ClassDB::bind_method(D_METHOD("set_octan_particle_count"),&ParticleBuffer::set_octan_particle_count);
    ClassDB::bind_method(D_METHOD("get_octan_particle_count"),&ParticleBuffer::get_octan_particle_count);    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "Octan Particle Count"), "set_octan_particle_count", "get_octan_particle_count");
    

    ClassDB::bind_method(D_METHOD("set_octan_max_depth"),&ParticleBuffer::set_octan_max_depth);
    ClassDB::bind_method(D_METHOD("get_octan_max_depth"),&ParticleBuffer::get_octan_max_depth);    
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "Octan Max Depth"), "set_octan_max_depth", "get_octan_max_depth");
    



}
      

ParticleBuffer::ParticleBuffer() 
{
    m_data = ParticleDataContainer();
    m_octree = LinearOctree();
}