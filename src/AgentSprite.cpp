//
//  AgentSprite.cpp
//  CS134-Project1
//
//  Created by Oscar Diaz Vega on 3/10/23.
//

#include "AgentSprite.hpp"

float AgentSprite::getBoundingCircleRadius(){
    return glm::sqrt((width / 2 * width / 2) + (height / 2 * height / 2));
}


void AgentSprite::setPlayerToChase(PlayerShape *user){
    player = user;
}
void AgentSprite::integrate(){
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    glm::vec3 vecToPlayer = player->pos - pos;
    float distanceSquared = glm::distance2(player->pos, pos);
    float gravity = -10.0;
    float forceStrength = gravity / distanceSquared;
    
    vecToPlayer += forceStrength;
    
    //linear motion
    forces = vecToPlayer;
    pos += velocity * dt;
    glm::vec3 accel = acceleration;
    accel += (1.0 / mass) * forces;
    velocity += accel * dt;
    velocity *= damping;
    
    //calc the rotation angle
    glm::vec3 vecFromAgentToPlayer = glm::normalize(player->pos - pos);
    glm::vec3 u = glm::vec3(0,-1,0); // up vector for the agent
    float angle = glm::orientedAngle(vecFromAgentToPlayer, u, glm::vec3(0,0,-1));
    rot = glm::degrees(angle);
    

    
}
