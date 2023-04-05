#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Shape.hpp"
#include "PlayerShape.hpp"
#include "AgentSprite.hpp"
#include "ParticleEmitter.hpp"
#include "Particle.hpp"

//TODO: figure out how to get agents to rotate to player, figure out why explosion effects can only be generated for only a few seconds (maybe the emitter dies after a few seconds?)



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

        ParticleEmitter agentEmitter;
        ParticleEmitter playerWeapon;
        ParticleEmitter agentPlayerCollisionEmitter;
    
    
        PlayerShape *player = NULL;
        ofImage defaultImage;
        ofImage playerImage;
        ofImage agentImage;
        ofImage bulletImage;
        ofVec3f mouse_last;
        bool backgroundLoaded = false;
        bool playerImageLoaded = false;
        bool agentImageLoaded = false;
        bool playJetpack = false;
        bool isPlayerShooting = false;
        bool gameStarted;
        bool gameOver = false;
        bool inFullScreen = false;
        float elapsedTime = 0;
        int resetLevel = 0;
        
        // Some basic UI
        //
        bool bHide;
        ofxFloatSlider rate;
        ofxToggle showPlayerImage;
        ofxToggle showAgentImage;
        ofxToggle showBackgroundImage;
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
};
