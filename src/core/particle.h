#pragma once

#include "core/particle_buffer.h"
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace PSS
{

    


    typedef unsigned int Particle; 
    
    
    void encode_buffer(const godot::Ref<PSS::ParticleBuffer> particleBuffer, godot::PackedByteArray& encodedArray);
    void encode_particle(const godot::Ref<PSS::ParticleBuffer> particleBuffer, const Particle particle, godot::PackedByteArray & encodedArray);


    void particle_to_partice_interaction(godot::Ref<ParticleBuffer> buffer, Particle p1, Particle p2, double delta);



 

} 

