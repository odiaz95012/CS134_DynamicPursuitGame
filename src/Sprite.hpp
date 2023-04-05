//
//  Sprite.hpp
//  CS134-Project1
//
//  Created by Oscar Diaz Vega on 3/6/23.
//
#pragma once

#include "Shape.hpp"
#include<cstdlib>
// Base class for a Sprite. Can be instantiated on it's own (deafult)
// or subclassed to create a custom sprite.  Includes Shape transformations.
// If the sprite has no image set on it, then a simple triangle will be drawn.
//

class Sprite : public Shape {
public:
    bool bHighlight = false;
    bool bSelected = false;
    bool bShowImage;
    
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float rotationSpeed = 2.0;
    float birthtime = 0; // elapsed time in ms
    float lifespan = -1;  //  time in ms
    string name =  "UnammedSprite";
    float width = 40;
    float height = 40;
    float speed = 2.0;
    
    ofImage spriteImage;
    
    // default verts for polyline shape if no image on sprite
    vector<glm::vec3> verts;
    Sprite() {
        // default geometry (triangle) if no image attached.
        //
        verts.push_back(glm::vec3(-5, 5, 0));
        verts.push_back(glm::vec3(5, 5, 0));
        verts.push_back(glm::vec3(0, -5, 0));
    }
    
    // some functions for highlighting when selected
    //
    void draw();
    
    float age() {
        return (ofGetElapsedTimeMillis() - birthtime);
    }
    
    void setImage(ofImage img) {
        spriteImage = img;
        bShowImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }
    
    virtual bool inside(const glm::vec3 p);
    virtual void update() {}
    
    bool insideTriangle(const glm::vec3 p);
    
    
    void setSelected(bool state) { bSelected = state; }
    void setHighlight(bool state) { bHighlight = state; }
    bool isSelected() { return bSelected; }
    bool isHighlight() { return bHighlight; }
    float getBoundingCircleRadius();
};



