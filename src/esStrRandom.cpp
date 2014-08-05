//
//  esStrRandom.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 13/06/2014.
//
//

#include "esStrRandom.h"

void esStrRandom::calCandidatePosition(ofxTileMap *solidMap, ofxTileMap *dynamicMap, bool &resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing){

    int next_pos_x = rand() % dynamicMap->getWidth();
    int next_pos_y = rand() % dynamicMap->getHeight();
    
    while (dynamicMap->getTileSafe( next_pos_x, next_pos_y) != WALKABLE) {
        next_pos_x = rand() % dynamicMap->getWidth();
        next_pos_y = rand() % dynamicMap->getHeight();
    }
    
    target.x = next_pos_x;
    target.y = next_pos_y;
    
    resetTarget = true;
    
}