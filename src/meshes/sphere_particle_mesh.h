#pragma once

#include "core/particle.h"
#include "core/particle_mesh.h"

namespace PSS
{
    class SphereParticleMesh : public ParticleMesh
    {

        GDCLASS(SphereParticleMesh,ParticleMesh)
        private:
            int _radius;
            std::vector<Particle> _particles;
            void regenerateMesh();


        protected:
            static void _bind_methods();

        
        
        public:
            void set_radius(int size);
            int get_radius();


            void set_particle(Particle _template);
            uint16_t get_particle();

            std::vector<Particle> get_particles();
            uint16_t get_particle_count();


            SphereParticleMesh(); 
    };
}