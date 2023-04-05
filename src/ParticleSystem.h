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
	int removeNear(const ofVec3f & point, float dist);
	void drawAgents();
    void drawParticles();
	vector<AgentSprite> agents;
    vector<Particle> particles;
	vector<ParticleForce *> forces;
    bool collisionOccured(AgentSprite *agent);
    bool checkPlayerShotAgent(Particle *particle, AgentSprite *agent);
    void setPlayerToChase(PlayerShape *user);
    void playExplosionSound();
    
    ofSoundPlayer playerLostEnergy;
    bool playerHitByAgent = false;
    bool playerHitAgent = false;
};



// Some convenient built-in forces
//
class GravityForce: public ParticleForce {
	ofVec3f gravity;
public:
	GravityForce(const ofVec3f & gravity);
	void updateForce(Particle *);
};

class TurbulenceForce : public ParticleForce {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f & min, const ofVec3f &max);
	void updateForce(Particle *);
};

class ImpulseRadialForce : public ParticleForce {
	float magnitude;
    float height;
public:
	ImpulseRadialForce(float magnitude); 
	void updateForce(Particle *);
    void setHeight(float height);
};

