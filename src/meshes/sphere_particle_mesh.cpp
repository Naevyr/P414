#include "meshes/sphere_particle_mesh.h"
using namespace PSS;






void SphereParticleMesh::_bind_methods()
{
  
    ClassDB::bind_method(D_METHOD("set_radius"),&SphereParticleMesh::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"),&SphereParticleMesh::get_radius);
    ClassDB::add_property("SphereParticleMesh",PropertyInfo(Variant::INT, "radius"), "set_radius", "get_radius");

    
    //ClassDB::bind_method(D_METHOD("set_particle"),&SphereParticleMesh::set_particle);
    //ClassDB::bind_method(D_METHOD("get_radius"),&SphereParticleMesh::get_radius);
    //ClassDB::add_property("SphereParticleMesh",PropertyInfo(Variant::INT, "radius"), "set_rendering_type", "get_rendering_type");



    //ClassDB::add_property("ParticleRenderer",PropertyInfo(Variant::INT, "rendering_type"), "set_rendering_type", "get_rendering_type");
}

void SphereParticleMesh::set_radius(int radius)
{

}

int SphereParticleMesh::get_radius()
{
    return 1;
}


SphereParticleMesh::SphereParticleMesh()
{

}