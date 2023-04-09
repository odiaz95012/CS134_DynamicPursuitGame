//
//  PlayerShape.hpp
//  CS134-Project1
//
//  Created by Oscar Diaz Vega on 3/8/23.
//

#ifndef PlayerShape_hpp
#define PlayerShape_hpp

#include <stdio.h>
#include "Shape.hpp"

class PlayerShape : public Shape {
public:
    typedef enum { Stopped, RotateLeft, RotateRight, MoveForward, MoveBackward } PlayerState;


    glm::vec3 heading() {
        //
        // 1) Create a rotation Matrix with rotation angle set to player's rotation in radians
        //    the "axis" we are rotating around is (0, 0, 1), which is z axis.
        //
        // 2) Multiply the players "up" vector (0, -1, 0) by the rotation matrix. The extra "1"
        //    at the end of the up vector is the homogenious coordinate since we are using a 4x4 matrix;
        //
        return glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
    }
    PlayerShape() {
        // default geometry (triangle) if no image attached.
        //
        verts.push_back(glm::vec3(-25, 25, 0));
        verts.push_back(glm::vec3(25, 25, 0));
        verts.push_back(glm::vec3(0, -25, 0));
        scale = glm::vec3(3,3,3);
        drawable = false;
        pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    }

    PlayerState state = Stopped;
    
    
    vector<glm::vec3> verts;
    bool bShowImage;
    
    glm::vec3 velocity = glm::vec3(1000, 1000, 0);
    float rotationSpeed = 3.0;
    float birthtime = 0; // elapsed time in ms
    float lifespan = -1;  //  time in ms
    string name =  "Player";
    bool isAlive;
    int nEnergy = 5; // Starting energy is 5, it's decremented by one every time the player is hit by an agent
    ofImage playerImage;
    float width = 200;
    float speed = 3.0;
    float height = 200;
    float totalSurvivalTime = 0;
    bool drawable;
    bool tookDamage = false;

    bool isInBounds(glm::vec3 p);
    void draw();
    void setImage(ofImage image);
    virtual bool inside(const glm::vec3 p);
    bool insideTriangle(const glm::vec3 p);
    float age() {
        return (ofGetElapsedTimeMillis() - birthtime);
    }
    void update();
    void moveUp();
    void moveLeft();
    void moveRight();
    void moveDown();
    void setLifespan(float lifespan);
    void setSpeed(float playerSpeed);
    float getBoundingCircleRadius();
    
    
};
#endif /* PlayerShape_hpp */
