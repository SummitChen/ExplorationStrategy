//
//  esStrFBMC.h
//  ExplorationStrategy
//
//  Created by Chen Si on 29/07/2014.
//
//

#ifndef ExplorationStrategy_esStrFBMC_h
#define ExplorationStrategy_esStrFBMC_h

#include "esStrategy.h"

class esStrFBMC : public esStrategy{
    
public:
    
    virtual void calCandidatePosition(ofxTileMap* solidMap, ofxTileMap* dynamicMap, bool& resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing);
    
    virtual void debugDraw();
    
    
private:
    
    void utilityDecision(std::map<double, KERNEL::Position>& resoultMap,
                         std::vector<KERNEL::Position> canPosVec,
                         std::set<esPolygon*> exploredPolygon,
                         ofVec2f& target,
                         bool  localSearch);
    
    ofxTileMap*      exploredMap;
    float            alpha, beta;
    Robot            scoutBot;
    
    ofMesh debugmesh;
    ofMesh debugMesh2;
    
    bool level1, level2, level3;
    ofVec2f circleCenter;
    float level1_radius;
    float level2_radius;
    float level3_radius;
    
};


#endif
