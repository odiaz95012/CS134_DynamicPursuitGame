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
    
    //rotational motion
    rot += (angularVelocity * dt);
    float a = angularAcceleration;;
    a += (angularForce * 1.0 / mass);
    angularVelocity += a * dt;
    angularVelocity *= damping;
}
