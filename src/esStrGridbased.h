//
//  esStrGridbased.h
//  ExplorationStrategy
//
//  Created by Chen Si on 19/07/2014.
//
//

#ifndef ExplorationStrategy_esStrGridbased_h
#define ExplorationStrategy_esStrGridbased_h

#include "esStrategy.h"

class esStrGridbased : public esStrategy{

public:
    
    virtual void calCandidatePosition(ofxTileMap* solidMap, ofxTileMap* dynamicMap, bool& resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing);
    
};


#endif
