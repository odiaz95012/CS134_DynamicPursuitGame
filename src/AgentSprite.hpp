//
//  AgentSprite.hpp
//  CS134-Project1
//
//  Created by Oscar Diaz Vega on 3/10/23.
//

#ifndef AgentSprite_hpp
#define AgentSprite_hpp

#include <stdio.h>
#include "Sprite.hpp"
#include "PlayerShape.hpp"


class AgentSprite : public Sprite {
public:
    ofImage agentImage;
    PlayerShape *player;
    bool imageLoaded;
    bool isDead = false;
    ofVec3f acceleration = glm::vec3(0,0,0);
    ofVec3f forces = glm::vec3(0,0,0);
    float damping = .99;
    float mass = 1.0;
    AgentSprite(){
        verts.push_back(glm::vec3(-20, 20, 0));
        verts.push_back(glm::vec3(20, 20, 0));
        verts.push_back(glm::vec3(0, -20, 0));
        name = "Agent Sprite";
    }

    float getBoundingCircleRadius();
    void integrate();
    void setPlayerToChase(PlayerShape *user);
    void setLifespan(float life)   { lifespan = life; }
};
#endif /* AgentSprite_hpp */
