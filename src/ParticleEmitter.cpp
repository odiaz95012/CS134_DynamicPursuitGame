
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
        particle.radius = particleRadius;
        particle.position = position;
        particle.birthtime = time;
        particle.lifespan = lifespan;
        particle.color = color;
        sys->add(particle);
    }
    break;
    case SphereEmitter:
        break;
    case DirectionalEmitter:{ // for player weapon emitter
            Particle particle;
            if(hasChildImage && showChildImage) particle.setImage(childImage);
            particle.setPlayerPointer(player);
            particle.velocity = velocity;
            particle.position = position;
            glm::vec3 perpenVec = glm::vec3(-player->heading().y, player->heading().x, 0);
            particle.forces = perpenVec * 500.0; // this is so that the when the player shoots, they come from the weapon, not the player itself
            particle.radius = particleRadius;
            particle.birthtime = time;
            particle.lifespan = lifespan;
            particle.color = color;
            sys->add(particle);
            blasterSound.play();
        break;
        }
    case AgentEmitter: // For emitter to spawn agents
        AgentSprite agent;
        agent.setPlayerToChase(player);
        if(hasChildImage && showChildImage) agent.setImage(childImage);
        agent.velocity = velocity;
        float randomXPoint = rand() % ofGetWidth();
        float randomYPoint = rand() % ofGetHeight();
        agent.pos = glm::vec3(randomXPoint, randomYPoint, 0);
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


