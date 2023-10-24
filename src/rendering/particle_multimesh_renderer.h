#include <godot_cpp/classes/multi_mesh_instance3d.hpp>



#include "core/particle_buffer.h"
namespace PSS
{
    class ParticleMultimeshRenderer : public godot::MultiMeshInstance3D
    {
        GDCLASS(ParticleMultimeshRenderer, godot::MultiMeshInstance3D)
        
        private:

            godot::Ref<ParticleBuffer> m_particleBuffer;
            bool m_initialized = false;

        protected:
            static void _bind_methods();
        
        public:

            ParticleMultimeshRenderer();
            void _ready();
            void _process(double delta);

    };
} // namespace name
