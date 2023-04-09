
// Kevin M.Smith - CS 134 SJSU

#include "ParticleSystem.h"

\
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
    try{
        agents.erase(agents.begin() + i);
    }catch(exception){
        cout << "failed to remove agent" << endl;
    }
}
void ParticleSystem::removeParticle(int i) {
    try{
        particles.erase(particles.begin() + i);
    }catch(exception){
        cout << "failed to remove particle" << endl;
    }
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
void ParticleSystem::updateAgents() {
	// check if empty and just return
	if (agents.size() == 0) return;

	vector<AgentSprite>::iterator agent = agents.begin();
	vector<AgentSprite>::iterator tmp;
    
	// check which particles and agents have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, we need to use an iterator.
    
	while (agent != agents.end()) {
		if ((agent->lifespan != -1 && agent->age() > agent->lifespan) || agent->isDead){
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
            player->tookDamage = true;
            agents[i].lifespan = -2;
            player->nEnergy -= 1;
        }
        
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
        if ((particle->lifespan != -1 && particle->age() > particle->lifespan) || particle->isDead){
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


