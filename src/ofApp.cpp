#include "ofApp.h"
#include <cstdlib>
//--------------------------------------------------------------
void ofApp::setup(){
    srand((unsigned) time(0)); // for the random spawn point of each agent

    ofSetVerticalSync(true);

    // create an image for sprites being spawned by emitter
    //
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
    if(!bulletImage.load("images/playerBulletImage.png")){
        cout << "Failed to load bullet image" << endl;
        ofExit();
    }
    bulletImage.resize(75, 75);
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
    ofSetBackgroundColor(ofColor::black);
    
    
    verdana16.load("verdana.ttf", 16, true, true);
    verdana16.setLineHeight(18.0f);
    verdana16.setLetterSpacing(1.037);

    
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
    gui.add(nEnergy.setup("nEnergy", 5, 1, 15));
    gui.add(nAgents.setup("nAgents", 1, 1, 10));


    player = new PlayerShape();
    
    playerWeapon.setVelocity(glm::vec3(0,0,0));
    playerWeapon.setEmitterType(DirectionalEmitter);
    playerWeapon.setOneShot(true);
    playerWeapon.setChildImage(bulletImage);
    playerWeapon.sys->setParticleLifespan(1000);
    playerWeapon.setGroupSize(1);
    playerWeapon.setPlayerPointer(player);
    
    if(!playerWeapon.blasterSound.load("sounds/blasterSound.mp3")){
        cout << "Failed to load blaster sound" << endl;
        ofExit();
    }

    
    agentEmitter.setVelocity(glm::vec3(0,0,0));
    agentEmitter.setEmitterType(AgentEmitter);
    agentEmitter.setGroupSize((int) nAgents);
    agentEmitter.setPlayerPointer(player);
    agentEmitter.setChildImage(agentImage);
    agentEmitter.setOneShot(false);
    
    if(!agentEmitter.sys->playerLostEnergy.load("sounds/damageHit.mp3")){
        cout << "Failed to load the player lost energy sound" << endl;
        ofExit();
    }
    agentEmitter.sys->playerLostEnergy.setLoop(false);
    

                                                    
    
    agentPlayerCollisionEmitter.setEmitterType(RadialEmitter);
    agentPlayerCollisionEmitter.setVelocity(glm::vec3(300,0,0));
    agentPlayerCollisionEmitter.setOneShot(true);
    agentPlayerCollisionEmitter.setGroupSize(100);
    agentPlayerCollisionEmitter.setParticleRadius(10.0);

    
    player->pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
    player->setImage(playerImage);
    playerWeapon.setPosition(player->pos);



    bHide = false;

}
void ofApp::playJetpackSound(){
    if(playJetpack && !playerJetpack.isPlaying()){
        //playerJetpack.play();
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
//--------------------------------------------------------------
void ofApp::update() {
    
    playGameMusic();
    elapsedTime = ofGetElapsedTimeMillis();
    
    agentEmitter.setRate(rate);
    agentEmitter.setGroupSize(nAgents);
    agentEmitter.setLifespan(life * 1000);
    for (int i = 0; i < agentEmitter.sys->agents.size(); i++) {

        // Get values from sliders and update sprites dynamically
        //
        float sc = agentScale;
        agentEmitter.sys->agents[i].bShowImage = showAgentImage;
        agentEmitter.sys->agents[i].scale = glm::vec3(sc, sc, sc);
        
    }
    agentEmitter.update();
    
    //if(!isPlayerShooting)
        //playerWeapon.stop();
    
    playerWeapon.setPosition(player->pos); // player weapon is tied to the player
    playerWeapon.update();
    
    agentPlayerCollisionEmitter.update();
    
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
        player->pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
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
            verdana16.drawString(gameOver, -bounds.width/2 + 50 , bounds.height/2 - 25);
        
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
            verdana16.drawString(gameOver, -bounds.width/2 + 50 , bounds.height/2 - 25);
        
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
            ofPushMatrix();
            ofSetColor(ofColor::red);
            string startPrompt = "Press start to begin";
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
            ofPushMatrix();
            ofSetColor(ofColor::red);
            string startPrompt = "Press start to begin";
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
            nEnergy = 10;
            life = 10;
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
        playJetpack = true;
        player->state = PlayerShape::RotateLeft;
        break;
    case OF_KEY_RIGHT:
        playJetpack = true;
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
        playJetpack = false;
        break;
    case OF_KEY_RIGHT:
        playJetpack = false;
        break;
    case OF_KEY_UP:
        playJetpack = false;
        break;
    case OF_KEY_DOWN:
        playJetpack = false;
        player->state = PlayerShape::Stopped;
        break;
    case 'w':
        //isPlayerShooting = false;
        playerWeapon.blasterSound.stop();
        //playerWeapon.sys->weaponParticles.clear();
        break;
    case 'c':
        break;
    case OF_KEY_SHIFT:
        break;
    case ' ':
        break;
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

