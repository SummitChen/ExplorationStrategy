//
//  Robot.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 27/05/2014.
//
//

#include "Robot.h"

Robot::Robot(){
    position = ofVec2f(0, 0);
    speed = 1;
    sight_range = 40;
    radius = 10;
    
}

Robot::Robot(ofVec2f _pos, float _speed, int sight)
: radius(10){
    position = _pos;
    speed = _speed;
    sight_range = sight;
}

ofVec2f Robot::getPosition(){

    return position;
}

void Robot::setPosition( ofVec2f _pos){
    position = _pos;
}

float Robot::getSpeed(){
    return speed;
}

void Robot::setSpeed( float _speed){
    speed = _speed;
}

void Robot::draw(){
    
	ofNoFill();
	glLineWidth(1);
	ofSetColor(0, 255, 0, 128);
	ofCircle(position.x, position.y, 10);
	ofCircle(position.x, position.y, 3);
	ofFill();
    
    ofDrawBitmapString("scout", position.x + 12, position.y + 3);
}

float Robot::x(){
    return position.x;
}

float Robot::y(){
    return position.y;
}

int   Robot::getSight(){
    return sight_range;
}

void Robot::setSight(int sight){
    sight_range = sight;
}

int  Robot::getRadius(){
    return radius;
}