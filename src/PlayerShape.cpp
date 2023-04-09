//
//  PlayerShape.cpp
//  CS134-Project1
//
//  Created by Oscar Diaz Vega on 3/8/23.
//

#include "PlayerShape.hpp"

void PlayerShape::draw(){
    if(drawable){
        if(bShowImage){
            ofPushMatrix();
            ofSetColor(ofColor::white);
            ofMultMatrix(getMatrix());
            playerImage.draw(-playerImage.getWidth() / 2 , -playerImage.getHeight() / 2.0);
            ofPopMatrix();
        }else{
            ofSetColor(ofColor::red);
            ofPushMatrix();
            ofMultMatrix(getMatrix());
            ofDrawTriangle(verts[0], verts[1], verts[2]);
            ofPopMatrix();
        }
    }
}
void PlayerShape::setImage(ofImage image){
    playerImage = image;
    bShowImage = true;
    width = image.getWidth();
    height = image.getHeight();
}
bool PlayerShape::insideTriangle(const glm::vec3 p){
    // transform the screen space point p to the triangle's local
    // oordinate system  (object space);  this will take into account any
    // rotation, translation or scale on the object.
    //
    glm::vec4 p2 = glm::inverse(getMatrix()) * glm::vec4(p, 1);

    glm::vec3 v1 = glm::normalize(verts[0] - p2);
    glm::vec3 v2 = glm::normalize(verts[1] - p2);
    glm::vec3 v3 = glm::normalize(verts[2] - p2);
    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
    if (a1 < 0 && a2 < 0 && a3 < 0) return true;
    else return false;
}
bool PlayerShape::inside(const glm::vec3 p){
    if (!bShowImage) {
        return insideTriangle (p);
    }

    // if player image attached, then first check if point is inside bounds of image
    // in object space.  If point is inside bounds, then make sure the point is in
    // opaque part of image.
    //
    glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    int w = playerImage.getWidth();
    int h = playerImage.getHeight();
    if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) { // b/c image coordinates start at top left of screen
        int x = s.x + w / 2;
        int y = s.y + h / 2;
        ofColor color = playerImage.getColor(x, y);
        return (color.a != 0);   // check if color is opaque (not the transparent background)
    }
    else return false;
}
float PlayerShape::getBoundingCircleRadius(){
    if(!bShowImage)
        return glm::sqrt((width / 2 * width / 2) + (height / 2 * height / 2));
    else
        //only the non-transparent part of the image 
        return glm::sqrt(width / 2 + height / 2);
}
bool PlayerShape::isInBounds(glm::vec3 pos){
    //return pos.x + width > 2 * width && pos.x + width < ofGetWidth() && pos.y + height > 2 * height && pos.y + height < ofGetHeight();
    return pos.x - width >= 0 && pos.x + width <= ofGetWidth() && pos.y - height >= 0 && pos.y + height <= ofGetHeight();
}

void PlayerShape::setLifespan(float lifespan) {
    lifespan = lifespan;
}

void PlayerShape::setSpeed(float playerSpeed) {
    speed = playerSpeed;
}
void PlayerShape::update(){
    if(nEnergy <= 0){
        isAlive = false;
    }
    switch (state) {
    case RotateLeft:
        rot -= rotationSpeed;
        break;
    case RotateRight:
        rot += rotationSpeed;
        break;
    case MoveForward:   // move along heading vector use r(t) = o + dt
        {
            glm::vec3 nextUpPos = pos + (heading() * speed);
            if(isInBounds(nextUpPos))
                pos += heading() * speed;
            break;
        }
    case MoveBackward:
        {
            glm::vec3 nextDownPos = pos + (-heading() * speed);
            if(isInBounds(nextDownPos))
                pos += -heading() * speed;
            break;
        }
    default:
        break;
    }
}
