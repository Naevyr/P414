#include "core/linear_octree.h"

#include <godot_cpp/variant/vector3.hpp>
#include <array>
using namespace PSS;
using namespace godot;






bool PSS::LinearOctree::Box::Intersects (Box a, Box b)
{
    return !(a.position.x - a.size > b.position.x + b.size ||
    a.position.x + a.size < b.position.x - b.size ||
    a.position.y - a.size > b.position.y + b.size ||
    a.position.y + a.size < b.position.y - b.size ||
    a.position.z - a.size > b.position.z + b.size ||
    a.position.z + a.size < b.position.z - b.size);
}


int LinearOctree::GetOctanLinearIndex(godot::Vector3 position) 
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


godot::Vector3 LinearOctree::GetOctanRelativePosition(int index)
{

    switch(index)
    {
        case 0: return godot::Vector3(1,1,1);
        case 1: return godot::Vector3(-1,1,1);
        case 2: return godot::Vector3(1,1,-1);
        case 3: return godot::Vector3(-1,1,-1);
        case 4: return godot::Vector3(1,-1,1);
        case 5: return godot::Vector3(-1,-1,1);
        case 6: return godot::Vector3(1,-1,-1);
        case 7: return godot::Vector3(-1,-1,-1);
    }

}



void PSS::LinearOctree::subdivide(Octan parent)
{   



    
    int particleCount = m_octans[parent].particleCount;

    std::vector<Particle> orderedParticles(particleCount * 8);
    std::vector<Box> orderedParticlesBoxes(particleCount * 8);
    int particleCountPerOctan[8] = {0};

    //Reorder per octans
    for (int i = m_octans[parent].particleStartIndex ; i < m_octans[parent].particleStartIndex + m_octans[parent].particleCount; i++)
    {
        Vector3 relativePos = m_particleBoxes[i].position - m_octans[parent].box.position;

        Vector3i normalized = Vector3i(Math::sign(relativePos.x),Math::sign(relativePos.y),Math::sign(relativePos.z));
       



        Octan relativeChild = GetOctanLinearIndex(normalized);

        orderedParticles[ m_octans[parent].particleCount * relativeChild + particleCountPerOctan[relativeChild]] = m_particles[i];
        orderedParticlesBoxes[ m_octans[parent].particleCount * relativeChild + particleCountPerOctan[relativeChild]] = m_particleBoxes[i];


        particleCountPerOctan[relativeChild] += 1;
    }
    


            ///Generates the actual octans and assings the ordered array to the main list
          
            Particle currentIndex = m_octans[parent].particleStartIndex;
            for(int o = 0; o < 8; o++)
            {
                OctanData childData{};
                
                childData.box = Box{m_octans[parent].box.position + GetOctanRelativePosition(o) * m_octans[parent].box.size  / 2.0f , m_octans[parent].box.size / 2.0f};
                
                childData.level = m_octans[parent].level + 1;
                childData.childCount = 0;
                childData.parent = parent;
               
                childData.particleStartIndex =  currentIndex;
                childData.particleCount =  particleCountPerOctan[o];
                
                
                Octan child = parent + o + 1;
                if(m_octans.size() <= child)
                {
                    m_octans.push_back(childData);
                 

                }
                else
                {
                    m_octans.insert(m_octans.begin() + child, childData);
                    
                }


                for (int i = 0; i < particleCountPerOctan[o]; i++)
                {
                    m_particles[currentIndex + i] = orderedParticles[o * m_octans[parent].particleCount + i ];
                    m_particleBoxes[currentIndex + i] = orderedParticlesBoxes[o * m_octans[parent].particleCount + i ];
                    
                }

                currentIndex += particleCountPerOctan[o];
                
            }


        
            
         


            m_octans[parent].subdivided = true;
            m_octans[parent].childCount += 8;
            

            
            
            for (int i = parent + m_octans[parent].childCount + 1; i <  m_octans.size(); i++)
            {
                
                if(m_octans[i].parent > parent)
                {
                    m_octans[i].parent += 8;
                    
                }
            }


            parent = m_octans[parent].parent;

           

            while (parent != UINT32_MAX)
            {


                m_octans[parent].childCount += 8;
                parent = m_octans[parent].parent;

                
            }

        
}

void LinearOctree::insertParticle(Octan octan, Particle particle, Box particleBox)
{

    
    unsigned int index = m_octans[octan].particleStartIndex + m_octans[octan].particleCount + 1;
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
    for(int i = octan + m_octans[octan].childCount + 1; i < m_octans.size(); i++)
    {
        m_octans[i].particleStartIndex += 1;
    }

    m_octans[octan].particleCount += 1 ;

 
    Octan parent = m_octans[octan].parent;
    


    while (parent != UINT32_MAX)
    {
        
        m_octans[parent].particleCount += 1;
        parent = m_octans[parent].parent;

    }


}

void LinearOctree::insert(Particle particle, Vector3 position, float size)
    {
        Octan octan = 0;
        Box particleBox{position,size};


        bool exploring = true;


        while (exploring)
        {
            //Particle out of bounds
            if(octan >= m_octans.size())
                return;

            if(!Box::Intersects(particleBox,m_octans[octan].box))
            {
                octan += m_octans[octan].childCount + 1; 
                continue;
            }
            
            

            
            if(m_octans[octan].subdivided)
            {
                octan += 1;
                continue;
            }
            
                        
            if(m_octans[octan].particleCount == m_nodeCapacity)
            {
                //Cannot add a particle when it's over level, this way we avoid trees too deep
                if(m_octans[octan].level == m_maxLevel)
                    return;
                subdivide(octan);
                octan += 1;
                continue;

            }
            
            
            
            
            insertParticle(octan,particle,particleBox);
          
            return;


            
        }

    }

std::vector<Particle> LinearOctree::query(Vector3 position, float size) const
{
    
    Box particleBox = Box{position,size};

    bool exploring = true;
    Octan octan = 0;

    
    int particleQueryCount  = 0;
    
    
    
    
    std::vector<Particle> queryResult{};
    
    while (exploring)
    {
        OctanData octanData = m_octans[octan];


        if(octan >= m_octans.size())
            return queryResult;



        
        

        if(!Box::Intersects(particleBox,m_octans[octan].box))
        {
            octan =+ octanData.childCount + 1; 
            continue;
        }

        
        if(octanData.subdivided)
        {
            octan += 1;
            continue;
        }
        
        
        

        
        
    
        for (int i = octanData.particleStartIndex; i <  octanData.particleStartIndex + octanData.particleCount; i++)
        {
            if(Box::Intersects(m_particleBoxes[i],particleBox))
            {
                
                queryResult.push_back(m_particles[i]);
               
            }
        }

        octan += 1;

    }

    return queryResult;

}


bool LinearOctree::is_initialized () const
{
    return m_initialized;
}
const std::vector<Particle>& LinearOctree::get_particle_array() const
{

    return m_particles;
}
const std::vector<LinearOctree::OctanData>& LinearOctree::get_octan_data() const
{
    return m_octans;
}


LinearOctree::LinearOctree(godot::Vector3 position, float size, unsigned int nodeCapacity, unsigned int maxLevel)
{

    m_octans = std::vector<OctanData>();

    m_particleBoxes = std::vector<Box>();
    m_particles = std::vector<Particle>();

    m_particleCount = 0;



    m_nodeCapacity = nodeCapacity;
    m_maxLevel = maxLevel;
    
    OctanData octanData = OctanData();
    octanData.level = 0;
    octanData.box = Box{position,size};

    octanData.parent = UINT32_MAX;
    
    
    m_octans.push_back(octanData);

    m_initialized = true;



}

LinearOctree::LinearOctree () {};
