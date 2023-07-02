#include "core/linear_octree.h"

#include <godot_cpp/variant/vector3.hpp>
#include <array>
using namespace PSS;
using namespace godot;
PSS::LinearOctree::Box::Box(godot::Vector3 p_position, float p_size)
{
    position = p_position;
    size = size;
}


bool PSS::LinearOctree::Box::Intersects (Box a, Box b)
{
    return !(a.position.x - a.size > b.position.x + b.size ||
    a.position.x + a.size < b.position.x - b.size ||
    a.position.y - a.size > b.position.y + b.size ||
    a.position.y + a.size < b.position.y - b.size ||
    a.position.z - a.size > b.position.z + b.size ||
    a.position.z + a.size < b.position.z - b.size);
}


int GetOctanLinearIndex(godot::Vector3i position) 
{
    

    if(position.x >=0 && position.y >=0 && position.z >=0) return 0;               //UNW
    else if(position.x -1 && position.y >=0 && position.z >=0) return  1;              //UNE
    else if(position.x >=0 && position.y >=0 && position.z -1) return  2;               //USW
    else if(position.x -1 && position.y >=0 && position.z -1) return  3;             //USE
    else if(position.x >=0 && position.y -1 && position.z >=0) return  4;              //DNW
    else if(position.x -1 && position.y -1 && position.z >=0) return  5;           //DNE
    else if(position.x >=0 && position.y -1 && position.z -1) return  6;            //DSW
    else if(position.x -1 && position.y -1 && position.z -1) return  7;          //DSE
}


godot::Vector3i GetOctanRelativePosition(int index)
{

    switch(index)
    {
        case 0: return godot::Vector3i(1,1,1);
        case 1: return godot::Vector3i(-1,1,1);
        case 2: return godot::Vector3i(1,1,-1);
        case 3: return godot::Vector3i(-1,1,-1);
        case 4: return godot::Vector3i(1,-1,1);
        case 5: return godot::Vector3i(-1,-1,1);
        case 6: return godot::Vector3i(1,-1,-1);
        case 7: return godot::Vector3i(-1,-1,-1);
    }

}



void PSS::LinearOctree::subdivide(Octan parent)
{   


    OctanDynamicData parentDynamicData = m_octanDynamicData[parent];
    OctanStaticData parentStaticData = m_octanStaticData[parent];
    
    int particleCount = parentDynamicData.particleCount;

    std::vector<Particle> orderedParticles(particleCount * 8);
    std::vector<Box> orderedParticlesBoxes(particleCount * 8);

    std::array<Octan,8> particleCountPerOctan = std::array<Octan,8>();


    //Reorder per octans
    for (int particle = parentDynamicData.particleStartIndex ; particle < parentDynamicData.particleStartIndex + parentDynamicData.particleCount; particle++)
    {
        Vector3 relativePos = m_particleBoxes[particle].position - parentStaticData.box.position;
        Vector3i normalized = Vector3i(0,0,0);

        normalized.x = Math::sign(relativePos.x);
        normalized.y = Math::sign(relativePos.y);      
        normalized.z = Math::sign(relativePos.z);
                        
        Octan octanChildIndex = GetOctanLinearIndex(normalized);

        orderedParticles[ parentDynamicData.particleCount * octanChildIndex + particleCountPerOctan[octanChildIndex]] = m_particles[particle];
        orderedParticlesBoxes[ parentDynamicData.particleCount * octanChildIndex + particleCountPerOctan[octanChildIndex]] = m_particleBoxes[particle];


        particleCountPerOctan[octanChildIndex] += 1;
    }
    


            ///Generates the actual octans and assings the ordered array to the main list
          
            Particle currentIndex = parentDynamicData.particleStartIndex;
            for(int o = 0; o < 8; o++)
            {
                OctanStaticData childStaticData{};
                OctanDynamicData childDynamicData{};
                childStaticData.box = Box(parentStaticData.box.position + GetOctanRelativePosition(o) * parentStaticData.box.size  / 2.0f ,parentStaticData.box.size / 2.0);
                
                childStaticData.level = parentStaticData.level + 1;
                childDynamicData.childCount = 0;
                childDynamicData.parent = parent;
               
                childDynamicData.particleStartIndex =  currentIndex;
                childDynamicData.particleCount =  particleCountPerOctan[o];
                
                
                Octan child = parent + o + 1;
                if(child <= m_octanDynamicData.size())
                {
                    m_octanDynamicData.push_back(childDynamicData);
                    m_octanStaticData.push_back(childStaticData);

                }
                else
                {
                    m_octanDynamicData.insert(m_octanDynamicData.begin() + child, childDynamicData);
                    m_octanStaticData.insert(m_octanStaticData.begin() + child, childStaticData);

                }


                for (int i = 0; i < particleCountPerOctan[o]; i++)
                {
                    m_particles[currentIndex + i] = orderedParticles[o * parentDynamicData.particleCount + i ];
                    m_particleBoxes[currentIndex + i] = orderedParticlesBoxes[o * parentDynamicData.particleCount + i ];
                    
                }

                currentIndex += particleCountPerOctan[o];
                
            }


            
            for (int i = parent + parentDynamicData.childCount; i <  m_octanDynamicData.size(); i++)
            {
                
                if(m_octanDynamicData[i].parent > parent)
                {
                    m_octanDynamicData[i].parent += 8;
                    
                }
            }

        
            
         


            parentDynamicData.subdivided = true;
            parentDynamicData.childCount += 8;
            //octans[parentIndex] = parent;

       
            parent = parentDynamicData.parent;

           

            while (parent != UINT32_MAX)
            {


                m_octanDynamicData[parent].childCount += 8;
                parent = m_octanDynamicData[parent].parent;

                /*
                
                
                OctanDynamicData grandParentData = m_octanDynamicData[parent]; 

                grandParentData.childCount += 8;

                octans[parentIndex] = grandParent;

            
               
                
                parent = octans[parentIndex].parent;
                */
            }

        
}

void LinearOctree::insertParticle(Octan octan, Particle particle, Box particleBox)
{

    
    unsigned int index = m_octanDynamicData[octan].particleStartIndex + m_octanDynamicData[octan].particleCount + 1;
    if(m_particles.size() <= index)
    {

        m_particles.push_back(particle);
        m_particleBoxes.push_back(particleBox); 
    }
    else
    {
        m_particles.insert(m_particles.begin() + index,particle);
        m_particleBoxes.insert(m_particleBoxes.begin() + index,particleBox);
    }


    //Translates all octans's range next to the current by one, since we are appending a particle behind all the particles of those octans
    for(int i = octan  + m_octanDynamicData[octan].childCount + 1; i < m_octanDynamicData.size(); i++)

    {
        
        m_octanDynamicData[i].particleStartIndex += 1;
       

    }
    
    
    

    

    m_octanDynamicData[octan].particleCount += 1 ;

    



    


    
    Octan parent = m_octanDynamicData[octan].parent;
    

    

    while (parent != UINT32_MAX)
    {
        
        m_octanDynamicData[parent].particleCount += 1;
        parent = m_octanDynamicData[octan].parent;
    }


}

LinearOctree::LinearOctree(godot::Vector3 position, float size, unsigned int nodeCapacity, unsigned int maxLevel)
{

    m_octanDynamicData = std::vector<OctanDynamicData>();
    m_octanStaticData = std::vector<OctanStaticData>();

    m_particleBoxes = std::vector<Box>();
    m_particles = std::vector<Particle>();

    m_particleCount = 0;



    m_nodeCapacity = nodeCapacity;
    m_maxLevel = maxLevel;
    
    OctanStaticData octanStaticData = OctanStaticData();
    octanStaticData.level = 0;
    octanStaticData.box = Box(position,size);
    OctanDynamicData octanDynamicData = OctanDynamicData();
    octanDynamicData.parent = UINT32_MAX;
    

    m_octanStaticData.push_back(octanStaticData);
    m_octanDynamicData.push_back(octanDynamicData);
}

void LinearOctree::insert(Particle particle, Vector3 position, float size)
        {
            Octan octan = 0;
            Box particleBox = Box(position,size);


            bool exploring = true;


            while (exploring)
            {
                //Particle out of bounds
                if(octan >= m_octanDynamicData.size())
                    return;

                if(!Box::Intersects(particleBox,m_octanStaticData[octan].box))
                {
                    octan += m_octanDynamicData[octan].childCount + 1; 
                    continue;
                }
                
                

                
                if(m_octanDynamicData[octan].subdivided)
                {
                    octan += 1;
                    continue;
                }
                
                            
                if(m_octanDynamicData[octan].particleCount == m_nodeCapacity)
                {
                    //Cannot add a particle when it's over level
                    if(m_octanStaticData[octan].level == m_maxLevel)
                        return;
                    subdivide(octan);
                    octan += 1;
                    continue;

                }
                
                
                
                
                insertParticle(octan,particle,particleBox);
                ///When is more we subdivide
                return;


               
            }

        }

std::vector<Particle> LinearOctree::query(Vector3 position, float size) const
{
    
    Box particleBox = Box(position,size);

    bool exploring = true;
    Octan octan = 0;

    
    int particleQueryCount  = 0;
    
    
    
    
    std::vector<Particle> queryResult{};
    
    while (exploring)
    {
        OctanDynamicData octanData = m_octanDynamicData[octan];


        if(octan >= m_octanDynamicData.size())
            return queryResult;



        
        

        if(!Box::Intersects(particleBox,m_octanStaticData[octan].box))
        {
            octan =+ octanData.childCount + 1; 
            continue;
        }

        
        if(octanData.subdivided)
        {
            octan += 1;
            continue;
        }
        
        
        octan += 1;
        

        
    
        for (int i = octanData.particleStartIndex; i <  octanData.particleStartIndex + octanData.particleCount; i++)
        {
            if(Box::Intersects(m_particleBoxes[i],particleBox))
            {
                
                queryResult.push_back(m_particles[i]);
               
            }
        }

        

    }

    return queryResult;

}






bool LinearOctree::is_initialized () const
{
    return m_initialized;
}

