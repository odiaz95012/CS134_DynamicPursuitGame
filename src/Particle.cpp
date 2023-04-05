#include "Particle.hpp"


Particle::Particle() {

    // initialize particle with some reasonable values first;
    //
    velocity.set(0, 0, 0);
    acceleration.set(0, 0, 0);
    position = glm::vec3(0, 0, 0);
    forces.set(0, 0, 0);
    lifespan = 5;
    birthtime = 0;
    radius = 10.0;
    damping = .99;
    mass = 1;
}
float Particle::getBoundingCircle(){
    if(hasImage)
        return glm::sqrt((particleImage.getWidth() / 2 * particleImage.getWidth() / 2) + (particleImage.getHeight() / 2 * particleImage.getHeight() / 2));

}
void Particle::draw() {
    if(hasImage){
        particleImage.draw(position.x + -particleImage.getWidth() / 2, position.y + -particleImage.getHeight() / 2.0);
    }else{
        ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
        ofDrawSphere(position, radius);
    }
}
void Particle::setImage(ofImage image){
    particleImage = image;
    hasImage = true;
}
void Particle::setPlayerPointer(PlayerShape *user){
    player = user;
}
// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

    
    // interval for this step
    //
    float dt = 1.0 / ofGetFrameRate();

    // update position based on velocity
    //


    position += (velocity * dt);

    // update acceleration with accumulated paritcles forces
    // remember :  (f = ma) OR (a = 1/m * f)
    //
    ofVec3f accel = acceleration;    // start with any acceleration already on the particle
    accel += (forces * (1.0 / mass));
    velocity += accel * dt;

    // add a little damping for good measure
    //
    velocity *= damping;

    // clear forces on particle (they get re-added each step)
    //
}

//  return age in seconds
//
float Particle::age() {
    return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


