#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Shape.hpp"
#include "PlayerShape.hpp"
#include "AgentSprite.hpp"
#include "ParticleEmitter.hpp"
#include "Particle.hpp"

//TODO: figure out particle and agent collisions



class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void setLevelParameters(int level);
        void playJetpackSound();
        void playGameMusic();
        void checkParticleAgentCollision();
        void updateEmitters();
        bool collisionOccured(AgentSprite *agent);
        bool playerTookDamage();
        void loadSoundEffects();
        void loadImages();
        void setupGui();
        void setupEmitters();
        void playGainedEnergySound();
        void drawStaticComponents(); // draws the game statistics/prompts

        ParticleEmitter agentEmitter;
        ParticleEmitter playerWeapon;
        ParticleEmitter agentPlayerCollisionEmitter;
    
    
        PlayerShape *player = NULL;
        ofImage defaultImage;
        ofImage playerImage;
        ofImage agentImage;
        ofImage rayImage;
        ofVec3f mouse_last;
        bool backgroundLoaded = false;
        bool playerImageLoaded = false;
        bool agentImageLoaded = false;
        bool gainedLife = false;
        bool drawGainEnergyStr = false;
        bool playJetpack = false;
        bool isPlayerShooting = false;
        bool gameStarted;
        bool gameOver = false;
        bool inFullScreen = false;
        bool noSpriteMode = false;
    
        float elapsedTime = 0;
        float gainedEnergyStrTime = 0;
        float gainedEnergyStrStart = 0;
        int gainEnergySoundPlayCount = 0;
        int resetLevel = 0;
        
        // Some basic UI
        //
        bool bHide;
        ofxFloatSlider rate;
        ofxToggle showPlayerImage;
        ofxToggle showAgentImage;
        ofxToggle showBackgroundImage;
        ofxToggle showAllSpriteImages;
    
        ofxFloatSlider life;
        ofxIntSlider playerSpeed;
        ofxIntSlider agentVelocity;
        ofxIntSlider nEnergy;
        ofxIntSlider nAgents;
        ofxIntSlider levelSelect;
        ofxLabel screenSize;
        ofxFloatSlider playerScale;
        ofxFloatSlider agentScale;
        

        ofTrueTypeFont verdana16;
        ofImage background;
        ofxPanel gui;
    
        ofSoundPlayer playerJetpack;
        ofSoundPlayer gameOverSound;
        ofSoundPlayer gameStartSound;
        ofSoundPlayer gameMusic;
        ofSoundPlayer playerLostEnergy;
        ofSoundPlayer playerGainedEnergy;
};
