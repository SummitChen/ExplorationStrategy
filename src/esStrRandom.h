//
//  esStrRandom.h
//  ExplorationStrategy
//
//  Created by Chen Si on 13/06/2014.
//
//

#ifndef ExplorationStrategy_esStrRandom_h
#define ExplorationStrategy_esStrRandom_h

#include "esStrategy.h"

class esStrRandom : public esStrategy{

public:
    
    virtual void calCandidatePosition(ofxTileMap* solidMap, ofxTileMap* dynamicMap, bool& resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing);
};

#endif
