#pragma once

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <vector>



#include "core/particle.h"


namespace PSS
{
    class LinearOctree
    {


        

        public: 
            struct Box
            {
                public:
                    godot::Vector3 position;
                    float size;
                    Box(){};
                    Box(godot::Vector3 p_position, float p_size);
                
                    static bool Intersects (Box a, Box b);

            };


            typedef unsigned int Octan;

            struct OctanStaticData
            {
                public:
                    Box box{godot::Vector3(),0.0};
                    unsigned int level = 0;
                    OctanStaticData() {}; 
            };

            struct OctanDynamicData
            {
                bool subdivided = false;
                unsigned int childCount = 0;
                unsigned int parent = UINT32_MAX;
                unsigned int particleStartIndex = 0;
                unsigned int particleCount = 0;

                static int GetOctanLinearIndex(godot::Vector3i position);
            
                
                static godot::Vector3i GetOctanRelativePosition(int index);
            
                

            };


        private:
            
            

            std::vector<Box> m_particleBoxes;
            std::vector<Particle> m_particles;
            
            

            std::vector<OctanStaticData> m_octanStaticData;
            std::vector<OctanDynamicData> m_octanDynamicData;
            

            unsigned int m_nodeCapacity;
            unsigned int m_maxLevel;

            unsigned int m_particleCount;


            
            bool m_initialized = false;

            
           

            void subdivide(Octan parent);
            void insertParticle(Octan octan, Particle particle, Box particleBox);


        public:
            LinearOctree(godot::Vector3 position, float size, unsigned int nodeCapacity, unsigned int maxLevel);

            void build();
            std::vector<Particle> query(godot::Vector3 position, float size) const;
            void insert(Particle particle, godot::Vector3 p_position, float p_size);
            

            bool is_initialized() const;













        
        
    };
}
