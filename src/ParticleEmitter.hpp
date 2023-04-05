#pragma once
#include "TransformObject.h"
#include "ParticleSystem.h"
#include "PlayerShape.hpp"
#include "Particle.hpp"

typedef enum { DirectionalEmitter, RadialEmitter, SphereEmitter, AgentEmitter } EmitterType;

class ParticleEmitter : public TransformObject {
public:
    ParticleEmitter();
    ParticleEmitter(ParticleSystem *s);
    ~ParticleEmitter();
    void init();
    void draw();
    void start();
    void stop();
    void setLifespan(const float life)   { lifespan = life; }
    void setVelocity(const ofVec3f &vel) { velocity = vel; }
    void setRate(const float r) { rate = r; }
    void setParticleRadius(const float r) { particleRadius = r; }
    void setEmitterType(EmitterType t) { type = t; }
    void setGroupSize(int s) { groupSize = s; }
    void setOneShot(bool s) { oneShot = s; }
    void setPlayerPointer(PlayerShape *user);
    bool checkParticleAgentCollision();
    void setChildImage(ofImage img);
    void update();
    void spawn(float time);
    ParticleSystem *sys;
    float rate;         // per sec
    bool oneShot;
    bool fired;
    ofVec3f velocity;
    float lifespan;     // sec
    bool started;
    float lastSpawned;  // ms
    float particleRadius;
    float radius;
    bool visible;
    int groupSize;      // number of particles to spawn in a group
    bool createdSys;
    EmitterType type;
    PlayerShape *player;
    ofImage childImage;
    bool hasChildImage = false;
    ofSoundPlayer blasterSound;
};
