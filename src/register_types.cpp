#include "register_types.h"


#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>


#include "core/elemental_spell_system.h"
#include "core/elemental_particle_resource.h"
#include "core/particle_data_container.h"
#include "core/particle_buffer.h"
#include "physics/particle_physics_system.h"
#include "rendering/particle_renderer.h"

using namespace godot;
using namespace PSS;



void initialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<ElementalParticleResource>();
    ClassDB::register_class<ElementalMeshResource>();

    ClassDB::register_class<ParticleBuffer>();

    
    ClassDB::register_class<ElementalSpellSystem>();
    ClassDB::register_class<ParticleRenderer>(); 
    ClassDB::register_class<ParticlePhysicsSystem>();
    
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(const GDExtensionInterface *p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

    init_obj.register_initializer(initialize_example_module);
    init_obj.register_terminator(uninitialize_example_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}