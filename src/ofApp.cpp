#include "ofApp.h"
#include <cstdlib>
//--------------------------------------------------------------
void ofApp::loadSoundEffects(){
    if(!playerJetpack.load("sounds/jetpack-sound.mp3")){
        cout << "Failed to load jetpack sound" << endl;
        ofExit();
    }
    playerJetpack.setLoop(true);
    
    if(!gameOverSound.load("sounds/gameOverSound.wav")){
        cout << "Failed to load game over sound" << endl;
        ofExit();
    }
    if(!gameStartSound.load("sounds/gameStartSound.ogg")){
        cout << "Failed to load game start sound" << endl;
        ofExit();
    }
    if(!gameMusic.load("sounds/gameMusic.mp3")){
        cout << "Failed to load game music" << endl;
        ofExit();
    }
    gameMusic.setLoop(true);
    gameMusic.setVolume(0.25f);
    if(!playerGainedEnergy.load("sounds/gainedEnergy.ogg")){
        cout << "Failed to load the healing sound effect" << endl;
        ofExit();
    }
    if(!playerWeapon.blasterSound.load("sounds/blasterSound.mp3")){
        cout << "Failed to load blaster sound" << endl;
        ofExit();
    }
    if(!playerLostEnergy.load("sounds/damageHit.mp3")){
        cout << "Failed to load the player lost energy sound" << endl;
        ofExit();
    }
    playerLostEnergy.setLoop(false);
    playerLostEnergy.setVolume(1.0f);
}
void ofApp::loadImages(){
    if (background.load("images/CS134-Project1Background.png")) {
        backgroundLoaded = true;
    }
    else {
        cout << "Can't open background image file" << endl;
        ofExit();
    }
    if(playerImage.load("images/CS134-Project1Player.png")){
        playerImageLoaded = true;
    }else{
        cout << "Can't open player image file" << endl;
        ofExit();
    }
    if(agentImage.load("images/CS134-Project1AgentSprite.png")){
        agentImageLoaded = true;
    }else{
        cout << "Failed to load agent image" << endl;
        ofExit();
    }
    if(!rayImage.load("images/playerBulletImage.png")){
        cout << "Failed to load bullet image" << endl;
        ofExit();
    }
    rayImage.resize(50, 50);
}
void ofApp::setupGui(){
    gui.setup();
    gui.add(rate.setup("rate", 0.5, 0.5, 5));
    gui.add(life.setup("Agent Lifespan", 3, 0, 10));
    gui.add(levelSelect.setup("Level", 1, 1, 3));
    gui.add(playerSpeed.setup("Player Speed", 5, 1, 10));
    gui.add(agentVelocity.setup("Agent Velocity",5, 1, 10));
    gui.add(playerScale.setup("Player Scale", 3.0, 1.0, 5.0));
    gui.add(agentScale.setup("Agent Scale", 4.0, 2.0, 8.0));
    gui.add(showPlayerImage.setup("Show Player Image", true));
    gui.add(showAgentImage.setup("Show Agent Image", true));
    gui.add(showBackgroundImage.setup("Show Background Image", true));
    gui.add(showAllSpriteImages.setup("Show Sprite Images", true));
    gui.add(nEnergy.setup("nEnergy", 5, 1, 15));
    gui.add(nAgents.setup("nAgents", 1, 1, 10));
}
void ofApp::setupEmitters(){
    playerWeapon.setVelocity(glm::vec3(0,0,0));
    playerWeapon.setEmitterType(DirectionalEmitter);
    playerWeapon.setOneShot(true);
    playerWeapon.setChildImage(rayImage);
    playerWeapon.setLifespan(1000);
    playerWeapon.setGroupSize(1);
    playerWeapon.setPlayerPointer(player);
    playerWeapon.setParticleColor(ofColor::aqua);
    
    agentEmitter.setVelocity(glm::vec3(0,0,0));
    agentEmitter.setEmitterType(AgentEmitter);
    agentEmitter.setGroupSize((int) nAgents);
    agentEmitter.setPlayerPointer(player);
    agentEmitter.setChildImage(agentImage);
    agentEmitter.setOneShot(false);
    agentEmitter.setLifespan(life * 1000);
    
    agentPlayerCollisionEmitter.setEmitterType(RadialEmitter);
    agentPlayerCollisionEmitter.setVelocity(glm::vec3(300,0,0));
    agentPlayerCollisionEmitter.setOneShot(true);
    agentPlayerCollisionEmitter.setGroupSize(50);
    agentPlayerCollisionEmitter.setParticleRadius(3.0);
    agentPlayerCollisionEmitter.setLifespan(1);
    agentPlayerCollisionEmitter.setParticleColor(ofColor::red);
    
    playerWeapon.setPosition(player->pos);
    glm::vec3 pos = glm::vec3(player->pos.x - 510, player->pos.y - 370, 0);
    agentPlayerCollisionEmitter.setPosition(pos);
}
void ofApp::setup(){
    srand((unsigned) time(0)); // for the random spawn point of each agent
    ofSetVerticalSync(true);
    
    loadImages();
    loadSoundEffects();
    
    ofSetBackgroundColor(ofColor::black);
    
    verdana16.load("verdana.ttf", 16, true, true);
    verdana16.setLineHeight(18.0f);
    verdana16.setLetterSpacing(1.037);
    
    setupGui();
    
    player = new PlayerShape();
    player->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    player->setImage(playerImage);

    setupEmitters();

    bHide = false;

}
bool ofApp::playerTookDamage(){
    playerLostEnergy.play();
    agentPlayerCollisionEmitter.start();
}
void ofApp::updateEmitters(){
    agentEmitter.update();
    playerWeapon.update();
    agentPlayerCollisionEmitter.update();
}
void ofApp::playJetpackSound(){
    if(playJetpack && !playerJetpack.isPlaying()){
        playerJetpack.play();
    }else if (!playJetpack && playerJetpack.isPlaying()){
        playerJetpack.stop();
    }
}
void ofApp::playGameMusic(){
    if(gameStarted && !gameOver && !gameMusic.isPlaying()){
        gameMusic.play();
    }else if((!gameStarted || gameOver) && gameMusic.isPlaying() ){
        gameMusic.stop();
    }
}

void ofApp::playGainedEnergySound(){
    if(!playerGainedEnergy.isPlaying() && gainEnergySoundPlayCount < 1){
        playerGainedEnergy.play();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    playGameMusic();
    elapsedTime = ofGetElapsedTimeMillis();
    
    if(!showAllSpriteImages){
        showPlayerImage = false;
        showAgentImage = false;
        playerWeapon.showChildImage = false;
        showBackgroundImage = false;
    }else{
        showPlayerImage = true;
        showAgentImage = true;
        playerWeapon.showChildImage = true;
        showBackgroundImage = true;
    }
    if(gainedLife){
        gainedEnergyStrTime = ofGetElapsedTimeMillis() - gainedEnergyStrStart;
    }
    if(gainedEnergyStrTime < 1000 && gainedEnergyStrTime != 0){
        drawGainEnergyStr = true;
        playGainedEnergySound();
        gainEnergySoundPlayCount++;
    }
    else{
        drawGainEnergyStr = false;
        gainedLife = false;
        gainedEnergyStrTime = 0;
        gainedEnergyStrStart = 0;
        gainEnergySoundPlayCount = 0;
    }
    if(player->tookDamage){
        playerTookDamage();
        player->tookDamage = false;
    }
    
    
    //agentPlayerCollisionEmitter.setLifespan(5);
    
    agentEmitter.setRate(rate);
    agentEmitter.setGroupSize(nAgents);
    agentEmitter.setLifespan(life * 1000);
    playerWeapon.setLifespan(1000);
    for (int i = 0; i < agentEmitter.sys->agents.size(); i++) {

        // Get values from sliders and update sprites dynamically
        //
        float sc = agentScale;
        agentEmitter.sys->agents[i].bShowImage = showAgentImage;
        agentEmitter.sys->agents[i].scale = glm::vec3(sc, sc, sc);
        
    }
    
    //if(!isPlayerShooting)
        //playerWeapon.stop();
    
    playerWeapon.setPosition(player->pos); // player weapon is tied to the player
    if(!inFullScreen){
        glm::vec3 pos = glm::vec3(player->pos.x - 510, player->pos.y - 370, 0);
        agentPlayerCollisionEmitter.setPosition(pos);
    }else{
        glm::vec3 pos = glm::vec3(player->pos.x - 755, player->pos.y - 500, 0);
        agentPlayerCollisionEmitter.setPosition(pos);
    }
    
    updateEmitters(); // update all emitters
    
    checkParticleAgentCollision();
    
    
    playJetpackSound();
    if(player->birthtime == 0 && gameStarted){ // set birthtime of player when the game is started
        player->birthtime = ofGetElapsedTimeMillis();
    }
    player->bShowImage = showPlayerImage;
    player->setSpeed(playerSpeed);

    float playerSc = playerScale;
    player->scale = glm::vec3(playerSc, playerSc, playerSc);
    player->update();
    if(player->nEnergy <= 0){ // check if player lost or not
        gameOverSound.play();
        gameOver = true;
        setLevelParameters(1);
        float endTime = elapsedTime;
        player->totalSurvivalTime = endTime - player->birthtime;
        agentEmitter.sys->agents.clear(); // kill all agents upon game over
        agentEmitter.stop();
    }
    if(!gameStarted || gameOver){ // allow player to adjust energy levels before game begins
        player->nEnergy = nEnergy;
        setLevelParameters(levelSelect);
    }
    if(!player->isAlive) // reset player to the center of the screen after loss
        player->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
}
void ofApp::drawStaticComponents(){
    if(gameOver && inFullScreen == false){
        //Display in the order: 'Game Over' -> 'Total Survival Time' -> 'Restart'
        ofSetColor(ofColor::red);
        ofPushMatrix();
            string restartPrompt = "Press space to restart";
            ofRectangle bounds = verdana16.getStringBoundingBox(restartPrompt, 0, 0);
            ofTranslate(bounds.width/2 + 420 , bounds.height / 2 + 420, 0);
            ofScale(2.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);
        
            string totalElapsedTime = "Total Survival Time: " + std::to_string((int)player->totalSurvivalTime / 1000) + " s";
            verdana16.drawString(totalElapsedTime, -bounds.width/2 - 15 , bounds.height/2);
        
            string gameOver = "Game Over";
            verdana16.drawString(gameOver, -bounds.width/2 + 55 , bounds.height/2 - 25);
        
            verdana16.drawString(restartPrompt, -bounds.width/2, bounds.height/2 + 25);
        ofPopMatrix();

    }else if (inFullScreen && gameOver){
        ofSetColor(ofColor::red);
        ofPushMatrix();
            string restartPrompt = "Press space to restart";
            ofRectangle bounds = verdana16.getStringBoundingBox(restartPrompt, 0, 0);
            ofTranslate(650 + bounds.width/2 , 520 + bounds.height / 2, 0);
            ofScale(2.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);
        
            string gameOver = "Game Over";
            verdana16.drawString(gameOver, -bounds.width/2 + 55 , bounds.height/2 - 25);
        
            string totalElapsedTime = "Total Survival Time: " + std::to_string((int)player->totalSurvivalTime / 1000) + " s";
            verdana16.drawString(totalElapsedTime, -bounds.width/2 - 15 , bounds.height/2);
            
            verdana16.drawString(restartPrompt, -bounds.width/2, bounds.height/2 + 25);

        ofPopMatrix();
    }
    //we draw the at different positions depending if user is in full screen mode or not
    if(inFullScreen == false && gameOver == false){
        if(showBackgroundImage)
            ofSetColor(ofColor::black);
        else
            ofSetColor(ofColor::red);

        string energyLevel = "Energy Level: " + std::to_string(player->nEnergy);
        verdana16.drawString(energyLevel, 100 , 30);
        string level = "Level " + std::to_string(levelSelect) + "/3";
        verdana16.drawString(level, 350 , 30);
        if(!gameStarted){
            string controls = "\t\t\t\t\t\tControls:\nPress the left and right arrow keys to turn\nPress the up and down arrow keys to move\n\t\t\tPress 'w' key to shoot";
            ofSetColor(ofColor::black);
            verdana16.drawString(controls, (ofGetWindowWidth() / 2.0) - 230, (ofGetWindowHeight() / 2.0) - 320);
            ofSetColor(ofColor::white);
            ofPushMatrix();
            ofSetColor(ofColor::red);
            string startPrompt = "Press space to begin";

            ofRectangle bounds = verdana16.getStringBoundingBox(startPrompt, 0, 0);
            
            ofTranslate(400 + bounds.width/2, 400 + bounds.height / 2, 0);
            ofScale(2.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);
            verdana16.drawString(startPrompt, -bounds.width/2, bounds.height/2 );
            ofPopMatrix();
        }
    }else if (inFullScreen == true && gameOver == false){
        if(showBackgroundImage)
            ofSetColor(ofColor::black);
        else
            ofSetColor(ofColor::red);
        string energyLevel = "Energy Level: " + std::to_string(player->nEnergy);
        verdana16.drawString(energyLevel, 100 , 30);
        string level = "Level " + std::to_string(levelSelect) + "/3";
        verdana16.drawString(level, 500 , 30);
        if(!gameStarted){
            string controls = "\t\t\t\t\t\tControls:\nPress the left and right arrow keys to turn\nPress the up and down arrow keys to move\n\t\t\tPress 'w' key to shoot";
            ofSetColor(ofColor::black);
            verdana16.drawString(controls, (ofGetWindowWidth() / 2.0) - 215, (ofGetWindowHeight() / 2.0) - 420);
            ofSetColor(ofColor::white);
            ofPushMatrix();
            ofSetColor(ofColor::red);
            string startPrompt = "Press space to begin";
            ofRectangle bounds = verdana16.getStringBoundingBox(startPrompt, 0, 0);
            
            ofTranslate(650 + bounds.width/2, 500 + bounds.height / 2, 0);
            ofScale(3.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);
            verdana16.drawString(startPrompt, -bounds.width/2, bounds.height/2 );
            ofPopMatrix();
        }
    }
    if(!inFullScreen){
        if(showBackgroundImage)
            ofSetColor(ofColor::black);
        else
            ofSetColor(ofColor::red);
        string elapsedTimeStr = "Elapsed Time: " + std::to_string((int)elapsedTime / 1000) + " s";
        verdana16.drawString(elapsedTimeStr, 500, 30);
        string frameRateStr = "Frame Rate: " + std::to_string((int)ofGetFrameRate()) + " fps";
        verdana16.drawString(frameRateStr, 730, 30);
    }else{
        if(showBackgroundImage)
            ofSetColor(ofColor::black);
        else
            ofSetColor(ofColor::red);
        string elapsedTimeStr = "Elapsed Time: " + std::to_string((int)elapsedTime / 1000) + " s";
        verdana16.drawString(elapsedTimeStr, 900, 30);
        string frameRateStr = "Frame Rate: " + std::to_string((int)ofGetFrameRate()) + " fps";
        verdana16.drawString(frameRateStr, 1200, 30);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(backgroundLoaded && showBackgroundImage)
        background.draw(glm::vec2(0,0), ofGetWidth(), ofGetHeight());
    if (!bHide) {
        gui.draw();
    }
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0);
    agentPlayerCollisionEmitter.draw();
    ofPopMatrix();
    
    if(drawGainEnergyStr){
        ofSetColor(ofColor::purple);
        string gainedEnergy = "+1 energy";
        verdana16.drawString(gainedEnergy, player->pos.x - 55, player->pos.y - 100);
        ofSetColor(ofColor::white);
    }
    drawStaticComponents();
    ofSetColor(ofColor::white);
    if(gameStarted && gameOver == false){
        player->draw();
        player->isAlive = true;
        agentEmitter.draw();
    }
    if(isPlayerShooting)
        playerWeapon.draw();
    ofSetColor(ofColor::white);
}
void ofApp::setLevelParameters(int level){
    switch(level){
        case 1: // easy level
            agentVelocity = 5;
            nAgents = 1;
            agentScale = 4.0;
            nEnergy = 10; // player health
            life = 10; // agent life span
            rate = 0.5;
            break;
        case 2: // medium level
            agentVelocity = 7;
            playerSpeed = 6.0;
            nAgents = 2;
            agentScale = 4.2;
            nEnergy = 5;
            life = 5;
            rate = 0.5;
            break;
        case 3: // hard level
            agentVelocity = 10;
            playerSpeed = 7.0;
            nAgents = 3;
            agentScale = 4.5;
            nEnergy = 3;
            life = 3;
            rate = 0.75;
            break;
    }
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
    switch (key) {
    case 'C':
    case 'c':
        agentPlayerCollisionEmitter.start();
        break;
    case 'F': // exit out of full screen
        ofToggleFullscreen();
        inFullScreen = false;
        break;
    case 'f': // enter full screen
        ofToggleFullscreen();
        inFullScreen = true;
        break;
    case 'H':
    case 'h':
        bHide = !bHide;
        break;
    case 'w':
        isPlayerShooting = true;
        playerWeapon.start();
        break;
    case OF_KEY_UP:
        playJetpack = true;
        player->state = PlayerShape::MoveForward;
        break;
    case OF_KEY_LEFT:
        player->state = PlayerShape::RotateLeft;
        break;
    case OF_KEY_RIGHT:
        player->state = PlayerShape::RotateRight;
        break;
    case OF_KEY_DOWN:
        playJetpack = true;
        player->state = PlayerShape::MoveBackward;
        break;
    case ' ':
        player->birthtime = 0;
        player->nEnergy = nEnergy;
        gameOver = false;
        gameStarted = true;
        gameStartSound.play();
        player->drawable = true; //spawn player
        agentEmitter.start();
        break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
    case OF_KEY_LEFT:
        player->state = PlayerShape::Stopped;
        break;
    case OF_KEY_RIGHT:
        player->state = PlayerShape::Stopped;
        break;
    case OF_KEY_UP:
        playJetpack = false;
        player->state = PlayerShape::Stopped;
        break;
    case OF_KEY_DOWN:
        playJetpack = false;
        player->state = PlayerShape::Stopped;
        break;
    case 'w':
        playerWeapon.blasterSound.stop();
        break;
    case 'c':
        break;
    case OF_KEY_SHIFT:
        break;
    case ' ':
        break;
    }
}
void ofApp::checkParticleAgentCollision(){
    if(agentEmitter.sys->agents.size() == 0 || playerWeapon.sys->particles.size() == 0) return;
    float particleRadius;
    float agentRadius;
    float distance;
    for(int i = 0; i < agentEmitter.sys->agents.size(); i++){
        AgentSprite *agent = &agentEmitter.sys->agents[i];
        agentRadius = agent->getBoundingCircleRadius();
        for(int j = 0; j < playerWeapon.sys->particles.size(); j++){
            Particle *particle = &playerWeapon.sys->particles[j];
            if(particle->hasImage) // if the particle has an image we use the width and height of the img
                particleRadius = particle->getBoundingCircle();
            else
                particleRadius = particle->radius;
            distance = glm::distance(agent->pos, particle->position);
            if(playerWeapon.hasChildImage){ // if the particle ray has an image we check if the particle is in the non-transparent part of the agent image
                if(distance < agentRadius + particleRadius && agent->inside(particle->position)){
                    gainedLife = true;
                    gainedEnergyStrStart = ofGetElapsedTimeMillis();
                    agent->isDead = true;
                    particle->isDead = true;
                    player->nEnergy += 1;
                }
            }else{
                if(distance < agentRadius + particleRadius){
                    gainedLife = true;
                    gainedEnergyStrStart = ofGetElapsedTimeMillis();
                    agent->isDead = true;
                    particle->isDead = true;
                    player->nEnergy += 1;
                }
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

