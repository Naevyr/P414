#include "register_types.h"


#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/core/class_db.hpp>




#include "core/elemental_spell_system.h"
#include "core/elemental_particle_resource.h"
#include "core/particle_data_container.h"
#include "core/particle_buffer.h"
#include "core/particle_data_container.h"
#include "rendering/particle_renderer.h"
#include "rendering/particle_multimesh_renderer.h"


using namespace godot;
using namespace PSS;



void initialize_p414(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<ElementalParticleResource>();
    ClassDB::register_class<ElementalMeshResource>();

    ClassDB::register_class<ParticleBuffer>();
    
    ClassDB::register_class<ParticleMultimeshRenderer>();
    
    ClassDB::register_class<ElementalSpellSystem>();
    ClassDB::register_class<ParticleRenderer>(); 
    ClassDB::register_class<ParticleDataContainer>();
}

void uninitialize_p414(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT p414_init(const GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_p414);
    init_obj.register_terminator(uninitialize_p414);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}