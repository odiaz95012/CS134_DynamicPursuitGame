//
//  Particle.hpp
//  CS134_Project2
//
//  Created by Oscar Diaz Vega on 4/4/23.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "PlayerShape.hpp"

class ParticleForceField;

class Particle {
public:
    Particle();

    glm::vec3 position;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofVec3f forces;
    float    damping;
    float   mass;
    float   lifespan;
    float   radius;
    float   birthtime;
    bool hasImage = false;
    bool isDead = false;
    void    integrate();
    void    draw();
    float   age();// sec
    void setImage(ofImage image);
    void setPlayerPointer(PlayerShape *user);
    float getBoundingCircle();
    ofImage particleImage;
    PlayerShape *player;
    ofColor color;
    
    
};



#endif /* Particle_hpp */
