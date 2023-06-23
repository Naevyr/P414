#include "core/particle.h"


using namespace godot;
using namespace PSS;



void encode_buffer(const Ref<ParticleBuffer> particleBuffer, godot::PackedByteArray & encodedArray)
{
    encodedArray.encode_u32(0,particleBuffer->get_particle_count());

    for (unsigned int particle = 0; particle < particleBuffer->get_particle_count(); particle++)
    {
        encode_particle(particleBuffer,particle,encodedArray,32 * particle + 4);
    }
    
}
void encode_particle(const Ref<ParticleBuffer> particleBuffer, const Particle particle, PackedByteArray & encodedArray, unsigned int startIndex)
{
 
    encodedArray.encode_var(startIndex,particleBuffer->get_position(particle));
    encodedArray.encode_float(startIndex + 12, particleBuffer->get_size(particle));

    Color color =  particleBuffer->get_particle_color(particle);
    encodedArray.encode_var(startIndex + 16,Vector3(color.r,color.g,color.b));
    

}


void particle_to_partice_interaction(PSS::ParticleBuffer& buffer, Particle p1, Particle p2, double delta)
{


}
