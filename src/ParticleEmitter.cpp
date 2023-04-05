
#include "ParticleEmitter.hpp"

ParticleEmitter::ParticleEmitter() {
    sys = new ParticleSystem();
    createdSys = true;
    init();
}
ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
    if (s == NULL)
    {
        cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
        ofExit();
    }
    sys = s;
    createdSys = false;
    init();
}
ParticleEmitter::~ParticleEmitter() {
    // deallocate particle system if emitter created one internally
    //
    if (createdSys) delete sys;
}
void ParticleEmitter::init() {
    rate = 1;
    velocity = glm::vec3(0,0,0);
    lifespan = 3000;
    started = false;
    oneShot = false;
    fired = false;
    lastSpawned = 0;
    radius = 1;
    particleRadius = 10.0;
    visible = true;
    type = DirectionalEmitter;
    groupSize = 1;
}
void ParticleEmitter::draw() {
        switch (type) {
        case DirectionalEmitter:
            sys->drawParticles();
        case SphereEmitter:
            break;
        case AgentEmitter:
            sys->drawAgents();
        case RadialEmitter:
            sys->drawParticles();
            break;
        default:
            break;
        }
}
void ParticleEmitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}
void ParticleEmitter::stop() {
    started = false;
    fired = false;
}
void ParticleEmitter::update() {
    float time = ofGetElapsedTimeMillis();
    if(type == DirectionalEmitter)
        checkParticleAgentCollision();
    if (oneShot && started){
        if (!fired) {
            // spawn a new particle(s)
            //
            for (int i = 0; i < groupSize; i++)
                spawn(time);
            lastSpawned = time;
        }
        fired = true;
        stop();
    }
    else if (((time - lastSpawned) > (1000.0 / rate)) && started) {
        // spawn a new particle(s)
        //
        
        for (int i = 0; i < groupSize; i++)
            spawn(time);
    
        lastSpawned = time;
    }
    if(type == AgentEmitter)
        sys->updateAgents();
    else
        sys->updateParticles();
}
void ParticleEmitter::setChildImage(ofImage img){
    childImage = img;
    hasChildImage = true;
}
bool ParticleEmitter::checkParticleAgentCollision(){
    if(sys->agents.size() == 0 || sys->particles.size() == 0) return false;
    cout << "checking for collision" << endl;
    float particleRadius;
    float agentRadius;
    float distance;
    for(int i = 0; i < sys->agents.size(); i++){
        agentRadius = sys->agents[i].getBoundingCircleRadius();
        for (int j = 0; j < sys->particles.size(); j++){
            particleRadius = sys->particles[j].radius;
            distance = glm::distance(sys->agents[i].pos, sys->particles[j].position);
            if(distance < particleRadius + agentRadius){
                sys->particles[j].lifespan = -2;
                sys->agents[i].lifespan = -2;
                player->nEnergy += 1;
            }
                return true;
        }
    }
    return false;
}
// spawn a single particle.  time is current time of birth
//
void ParticleEmitter::spawn(float time) {

    // set initial velocity and position
    // based on emitter type
    //
    switch (type) {
    case RadialEmitter:
    {
        Particle particle;
        ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        float speed = velocity.length();
        particle.velocity = dir.getNormalized() * speed;
        particle.position = position;
        sys->add(particle);
    }
    break;
    case SphereEmitter:
        break;
    case DirectionalEmitter:{ // for player weapon emitter
            Particle particle;
            if(hasChildImage) particle.setImage(childImage);
            particle.setPlayerPointer(player);
            particle.velocity = velocity;
            particle.position = position;
            glm::vec3 perpenVec = glm::vec3(-player->heading().y, player->heading().x, 0);
            particle.forces = perpenVec * 500.0; // this is so that the when the player shoots, they come from the weapon, not the player itself
            particle.radius = particleRadius;
            particle.lifespan = lifespan;
            sys->add(particle);
            blasterSound.play();
        break;
        }
    case AgentEmitter: // For emitter to spawn agents
        AgentSprite agent;
        agent.setPlayerToChase(player);
        if(hasChildImage) agent.setImage(childImage);
        agent.velocity = velocity;
        float randomXPoint = rand() % ofGetWidth();
        float randomYPoint = rand() % ofGetHeight();
        agent.pos = glm::vec3(randomXPoint, randomYPoint, 0);
        if(player->pos.x < agent.pos.x) // agent is to the right of the player so we rotate left
            agent.angularForce = -75.0;
        else if(player->pos.x > agent.pos.x) // agent is to the left of the player so we rotate right
            agent.angularForce = 75.0;
        agent.birthtime = time;
        agent.lifespan = lifespan;
        // add to system
        sys->add(agent);
        break;
    }
}
void ParticleEmitter::setPlayerPointer(PlayerShape *user){
    player = user;
    sys->player = user;
}


