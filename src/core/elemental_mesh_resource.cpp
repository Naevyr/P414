#include "core/elemental_mesh_resource.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_data_tool.hpp>


using namespace PSS;
using namespace godot;



void ElementalMeshResource::_bind_methods()
{

    ClassDB::bind_method(D_METHOD("generate_mesh"),&ElementalMeshResource::generate_mesh);

    ClassDB::bind_method(D_METHOD("set_position_array"),&ElementalMeshResource::set_position_array);
    ClassDB::bind_method(D_METHOD("get_position_array"),&ElementalMeshResource::get_position_array);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::PACKED_VECTOR3_ARRAY, "position_array"), "set_position_array", "get_position_array");
    
    ClassDB::bind_method(D_METHOD("set_convex_mesh"),&ElementalMeshResource::set_convex_mesh);
    ClassDB::bind_method(D_METHOD("get_convex_mesh"),&ElementalMeshResource::get_convex_mesh);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "convex_mesh",
        PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_convex_mesh", "get_convex_mesh");
    

    ClassDB::bind_method(D_METHOD("set_sample_size"),&ElementalMeshResource::set_sample_size);
    ClassDB::bind_method(D_METHOD("get_sample_size"),&ElementalMeshResource::get_sample_size);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT,"sample_size"), "set_sample_size", "get_sample_size");
    
    
    
    
    ClassDB::bind_method(D_METHOD("set_template_particle"),&ElementalMeshResource::set_template_particle);
    ClassDB::bind_method(D_METHOD("get_template_particle"),&ElementalMeshResource::get_template_particle);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "template_particle",
        PROPERTY_HINT_RESOURCE_TYPE, "ElementalParticleResource"), "set_template_particle", "get_template_particle");
    



}



void ElementalMeshResource::generate_mesh(float size)
{
    if(size <= 0.001)
    {
        ERR_PRINT("ElementalMeshResource: Size must be greater than 0.001");
        return;
    }

    m_meshNormals = static_cast<PackedVector3Array>(m_convexMesh->surface_get_arrays(0)[(int)Mesh::ARRAY_NORMAL]);
    m_meshPositions =  static_cast<PackedVector3Array>(m_convexMesh->surface_get_arrays(0)[(int)Mesh::ARRAY_VERTEX]);
    

    m_sampleSize = Math::min(m_meshPositions.size(), (int64_t) Math::clamp(m_sampleSize,150,10000));
        
    generateParticlePositions(Vector3(0,0,0), size);
}

bool ElementalMeshResource::isPointInsideMesh(Vector3 point)
{
    float ratio =  (float) m_meshPositions.size() / m_sampleSize;
    
    for (int i = 0; i < m_sampleSize ; i++)
    {
        Vector3 dir = (m_meshPositions[(int) (i * ratio)]-point).normalized();


        if(dir.dot(m_meshNormals[(int)(i * ratio)]) <=  0)
            return false;
    }

    return true;
}

void ElementalMeshResource::generateParticlePositions(Vector3 position, float size)
{
    
    
    m_exportPositions.clear();

    if(!isPointInsideMesh(position))
        return;
    generatePositionsVertically(position, true, size);
    generatePositionsVertically(position, false, size);


    
}
void ElementalMeshResource::generatePositionsVertically(Vector3 tempPos, bool upwards, float size)
{
    //Offset for spawning a new particle
    Vector3 deltaX = Vector3(1,0,0) *  size;
    Vector3 deltaY = (upwards ? Vector3(0,1,0) : Vector3(0,-1,0)) * size;
    Vector3 deltaZ = Vector3(0,0,1) * size;

    if(!upwards)
        //DeltaY is already inverted
        tempPos += deltaY;

    Vector3 tempYpos = tempPos;
    //Explore Y axis
    while(isPointInsideMesh(tempPos))
    {

        m_exportPositions.append(tempPos);
        Vector3 posPreXExploration = tempPos;
        tempPos += deltaX;
        //X
        while(isPointInsideMesh(tempPos))
        {
            m_exportPositions.append(tempPos);


            Vector3 posPreZExploration = tempPos;
            tempPos += deltaZ;
            //Z
            while(isPointInsideMesh(tempPos))
            {

                m_exportPositions.append(tempPos);
                tempPos += deltaZ;




            }
            tempPos = posPreZExploration;
            tempPos -= deltaZ;
            //-Z
            while(isPointInsideMesh(tempPos))
            {
                
                m_exportPositions.append(tempPos);
                tempPos -= deltaZ;
                
                
            }

            tempPos = posPreZExploration;
            tempPos += deltaX;
            

        }

        tempPos = posPreXExploration;
        tempPos -= deltaX;
        //-X
        while(isPointInsideMesh(tempPos))
        {
            m_exportPositions.append(tempPos);


            Vector3 tempZpos = tempPos;
            tempPos += deltaZ;
            //Z
            while(isPointInsideMesh(tempPos))
            {

                m_exportPositions.append(tempPos);
                tempPos += deltaZ;




            }
            tempPos = tempZpos;
            tempPos -= deltaZ;
            //-Z
            while(isPointInsideMesh(tempPos))
            {
                
                m_exportPositions.append(tempPos);
                tempPos -= deltaZ;
                
                
            }
            tempPos = tempZpos;
            tempPos -= deltaX;
        }
        

        tempPos = posPreXExploration;
        tempPos += deltaY;
    }

}

void ElementalMeshResource::set_position_array(godot::PackedVector3Array array)
{
    m_exportPositions = array;
}
godot::PackedVector3Array ElementalMeshResource::get_position_array() const
{
    return m_exportPositions;
}

void ElementalMeshResource::set_convex_mesh(Ref<Mesh> mesh)
{
    m_convexMesh = mesh;
}
godot::Ref<godot::Mesh> ElementalMeshResource::get_convex_mesh() const
{
    return m_convexMesh;
}

void ElementalMeshResource::set_sample_size( int size)
{
    m_sampleSize = size;
}
    int ElementalMeshResource::get_sample_size() const
{
    return m_sampleSize;
}

void ElementalMeshResource::set_template_particle(godot::Ref<ElementalParticleResource> templateParticle)
{
    m_templateParticle = templateParticle;
}

godot::Ref<ElementalParticleResource> ElementalMeshResource::get_template_particle() const
{
    return m_templateParticle;
}


ElementalMeshResource::ElementalMeshResource()
{   

}