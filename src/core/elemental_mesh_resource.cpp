#include "core/elemental_mesh_resource.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_data_tool.hpp>


using namespace PSS;
using namespace godot;



void ElementalMeshResource::_bind_methods()
{
}

ElementalMeshResource::ElementalMeshResource()
{   
    m_meshNormals = static_cast<PackedVector3Array>(m_convexMesh->surface_get_arrays(0)[(int)Mesh::ARRAY_NORMAL]);
    m_meshPositions =  static_cast<PackedVector3Array>(m_convexMesh->surface_get_arrays(0)[(int)Mesh::ARRAY_VERTEX]);
    

    m_sampleSize = Math::min(m_meshPositions.size(),(int64_t) m_sampleSize);
        
        
    generateParticlePositions(Vector3(0,0,0));

}

void ElementalMeshResource::generate_mesh()
{

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

void ElementalMeshResource::generateParticlePositions(Vector3 position)
{
    
    


    if(!isPointInsideMesh(position))
        return;
    generatePositionsVertically(position,true);
    generatePositionsVertically(position,false);


    
}
void ElementalMeshResource::generatePositionsVertically(Vector3 tempPos, bool upwards)
{
    //Offset for spawning a new particle
    Vector3 deltaX = Vector3(1,0,0) *  m_templateParticle->get_size() * 2;
    Vector3 deltaY = (upwards ? Vector3(0,1,0) : Vector3(0,-1,0)) * m_templateParticle->get_size() * 2;
    Vector3 deltaZ = Vector3(0,0,1) * m_templateParticle->get_size() * 2;

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