//
//  Robot.h
//  ExplorationStrategy
//
//  Created by Chen Si on 27/05/2014.
//
//

#ifndef ExplorationStrategy_Robot_h
#define ExplorationStrategy_Robot_h

#include "ofMain.h"

class Robot{
public:
    Robot();
    Robot(ofVec2f pos, float speed, int sight);
    
    float getSpeed();
    void  setSpeed(float sp);
    
    ofVec2f getPosition();
    void  setPosition(ofVec2f pos);
    
    float x();
    float y();
    
    int   getRadius();
    
    int   getSight();
    void  setSight(int sight);

    void  draw();
protected:
    
    ofVec2f position;
    float   speed;
    int     sight_range;
    int     radius;

};

#endif
