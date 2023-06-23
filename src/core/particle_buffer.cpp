#include "core/particle_buffer.h"

#include <stdexcept>

using namespace PSS;
using namespace godot;

void ParticleBuffer::_bind_methods()
{

}
      
Particle ParticleBuffer::add_particle(const Ref<ElementalParticleResource> particleTemplate, const Vector3 position)
{
    if(!m_octree.is_initialized())
        m_octree = LinearOctree(Vector3(0,0,0),m_bounds->get_size()[m_bounds->get_size().max_axis_index()],150,500);


    Particle particle = ParticleDataContainer::add_particle(particleTemplate,position);
    m_octree.insert(particle,position,particleTemplate->get_size());
    m_particleCount += 1;


    return particle;
}
std::vector<Particle> ParticleBuffer::add_cluster(const Ref<ElementalParticleResource> particleTemplate, const Ref<PackedVector3Array> positions)
{

    std::vector<Particle> particles{};
    for (size_t i = 0; i < positions->size(); i++)
    {
        Particle particle = add_particle(particleTemplate,positions->operator[](i));
        particles.push_back(particle);
    }
    



}

void ParticleBuffer::add_mesh(const Ref<ElementalMeshResource> mesh)
{
    //Finisci di convertire tutte le robe ref, funzioni che mancano e comincia a vedere il rendering se e come funziona
}



unsigned int ParticleBuffer::get_particle_count() const
{
    return m_particleCount;
}


std::vector<Particle> ParticleBuffer::get_nearby_particles(Vector3 position, float size) const
{
    return m_octree.query(position,size);
}



Ref<BoxMesh> ParticleBuffer::get_convex_bounds() const
{
    return m_bounds;
}

void ParticleBuffer::set_convex_bounds(Ref<BoxMesh> convexBounds)
{

    
    m_bounds = convexBounds;
}
