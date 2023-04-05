
// Kevin M.Smith - CS 134 SJSU

#include "ParticleSystem.h"

void ParticleSystem::playExplosionSound(){
    if(playerHitByAgent)
        playerLostEnergy.play();
    else
        playerLostEnergy.stop();
}
void ParticleSystem::add(const AgentSprite &p) {
	agents.push_back(p);
}
void ParticleSystem::add(const Particle &p) {
    particles.push_back(p);
}
void ParticleSystem::addForce(ParticleForce *f) {
	forces.push_back(f);
}

void ParticleSystem::removeAgent(int i) {
	agents.erase(agents.begin() + i);
}
void ParticleSystem::removeParticle(int i) {
    particles.erase(particles.begin() + i);
}
void ParticleSystem::setAgentLifespan(float l) {
	for (int i = 0; i < agents.size(); i++) {
        agents[i].setLifespan(l);
	}
}
void ParticleSystem::setParticleLifespan(float l) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].lifespan = l;
    }
}

void ParticleSystem::reset() {
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->applied = false;
	}
}
void ParticleSystem::setPlayerToChase(PlayerShape *user){
    player = user;
}
bool ParticleSystem::collisionOccured(AgentSprite *agent){
    float agentBoundedCircle = agent->getBoundingCircleRadius();
    float playerBoundedCircle = player->getBoundingCircleRadius();
    float distance = glm::distance(player->pos, agent->pos);
    if(player->bShowImage){
        // if the closest point of the agent's bounding circle overlaps with the non-transparent portion of the player's image
        return distance < agentBoundedCircle + playerBoundedCircle && player->inside(agent->pos);
    }else{
        //if the bounding circles of the agent and player overlap
        return distance < agentBoundedCircle + playerBoundedCircle;
    }
}
bool ParticleSystem::checkPlayerShotAgent(Particle *particle, AgentSprite *agent){
    float particleRadius = particle->getBoundingCircle();
    float agentBoundedCircle = agent->getBoundingCircleRadius();
    float distance = glm::distance(particle->position, agent->pos);
    return distance < particleRadius + agentBoundedCircle;
}
void ParticleSystem::updateAgents() {
	// check if empty and just return
	if (agents.size() == 0) return;

	vector<AgentSprite>::iterator agent = agents.begin();
	vector<AgentSprite>::iterator tmp;
    
	// check which particles and agents have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, we need to use an iterator.
    
	while (agent != agents.end()) {
		if ((agent->lifespan != -1 && agent->age() > agent->lifespan) || agent->lifespan == -2){
			tmp = agents.erase(agent);
            agent = tmp;
		}
		else agent++;
	}


	// integrate all the agents in the store
	//
    for (int i = 0; i < agents.size(); i++){
        AgentSprite *agent = &agents[i];
		agents[i].integrate();
        if(collisionOccured(agent)){ // if collision occured, then the agent is removed from the game and
            playerHitByAgent = true;
            playExplosionSound();
            agents[i].lifespan = -2;
            player->nEnergy -= 1;
        }
        playerHitByAgent = false;
        
    }

}
void ParticleSystem::updateParticles() {
    // check if empty and just return
    if (particles.size() == 0) return;
    vector<Particle>::iterator particle = particles.begin();
    vector<Particle>::iterator tmp;
    
    // check which particles and agents have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, we need to use an iterator.
    
    while (particle != particles.end()) {
        if ((particle->lifespan != -1 && particle->age() > particle->lifespan) || particle->lifespan == -2){
            tmp = particles.erase(particle);
            particle = tmp;
        }
        else particle++;
    }

    // update forces on all particles first
    //
    for (int i = 0; i < particles.size(); i++) {
        for (int k = 0; k < forces.size(); k++) {
            if (!forces[k]->applied)
                forces[k]->updateForce(&particles[i]);
        }
    }

    // update all forces only applied once to "applied"
    // so they are not applied again.
    //
    for (int i = 0; i < forces.size(); i++) {
        if (forces[i]->applyOnce)
            forces[i]->applied = true;
    }

    // integrate all the agents in the store
    //
    for (int i = 0; i < particles.size(); i++){
        Particle *particle = &particles[i];
        particles[i].integrate();
    }

}
// remove all particlies within "dist" of point (not implemented as yet)
//
int ParticleSystem::removeNear(const ofVec3f & point, float dist) { return 0; }

//  draw the particle cloud
//
void ParticleSystem::drawAgents() {
	for (int i = 0; i < agents.size(); i++) {
		agents[i].draw();
	}
}
void ParticleSystem::drawParticles(){
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
    }
}

// Gravity Force Field 
//
GravityForce::GravityForce(const ofVec3f &g) {
	gravity = g;
}

void GravityForce::updateForce(Particle *particle) {
	//
	// f = mg
	//
    particle->forces += gravity * particle->mass;
}

// Turbulence Force Field 
//
TurbulenceForce::TurbulenceForce(const ofVec3f &min, const ofVec3f &max) {
	tmin = min;
	tmax = max;
}

void TurbulenceForce::updateForce(Particle *particle) {
	//
	// We are going to add a little "noise" to a particles
	// forces to achieve a more natual look to the motion
	//
    particle->forces.x += ofRandom(tmin.x, tmax.x);
    particle->forces.y += ofRandom(tmin.y, tmax.y);
    particle->forces.z += ofRandom(tmin.z, tmax.z);
}

// Impulse Radial Force - this is a "one shot" force that
// eminates radially outward in random directions.
//
ImpulseRadialForce::ImpulseRadialForce(float magnitude) {
	this->magnitude = magnitude;
	applyOnce = true;
}

void ImpulseRadialForce::updateForce(Particle *particle) {

	// we basically create a random direction for each particle
	// the force is only added once after it is triggered.
	//
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    particle->forces += dir.getNormalized() * magnitude;
}
void ImpulseRadialForce::setHeight(float height){
    this->height = height;
    
}
