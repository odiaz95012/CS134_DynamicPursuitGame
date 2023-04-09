#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "ofMain.h"
#include "AgentSprite.hpp"
#include "PlayerShape.hpp"
#include "Particle.hpp"

//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(AgentSprite *) = 0;
    virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
    PlayerShape *player;
	void add(const AgentSprite &);
    void add(const Particle &);
	void addForce(ParticleForce *);
	void removeAgent(int);
    void removeParticle(int);
	void updateAgents();
    void updateParticles();
	void setAgentLifespan(float);
    void setParticleLifespan(float);
	void reset();
	void drawAgents();
    void drawParticles();
	vector<AgentSprite> agents;
    vector<Particle> particles;
	vector<ParticleForce *> forces;
    bool collisionOccured(AgentSprite *agent);
    void setPlayerToChase(PlayerShape *user);
    
};



