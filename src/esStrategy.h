//
//  esStrategy.h
//  ExplorationStrategy
//
//  Created by Chen Si on 13/06/2014.
//
//

#ifndef ExplorationStrategy_esStrategy_h
#define ExplorationStrategy_esStrategy_h

#include "ofxTileMap.h"
#include "Robot.h"
#include <cmath>
#include "ofxMapRouting.h"
#include "esPolygon.h"
#include "Position.h"
#include "DoubleList.h"
#include <iostream>
#include <fstream>

struct esNode{
    KERNEL::Position pos;
    bool             visitFlag;
};

struct edgeNode{
    KERNEL::Position pos;
    unsigned int     explorationFlag;
};

typedef std::vector<esNode> NodeVec;

class esStrategy{

public:
        
    virtual void calCandidatePosition(ofxTileMap* solidMap, ofxTileMap* dynamicMap, bool& resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing){}
    
    virtual void debugDraw(){}
    
    
protected:
    
    float calculateTilePercentage(ofxTileMap* dynamicMap, Robot& scout, ofVec2f& target);
    
    float calculateSegPercentage(ofxTileMap* dynamicMap, Robot& scout, ofVec2f& target);
    
    float calculateFeaPercentage(ofxTileMap* dynamicMap, Robot& scout, ofVec2f& target);
    
    void  calculateExploredPolygons(ofxTileMap* dynamicMap, std::list<esPolygon>& exploredPolygons, std::list<esPolygon>& unexploredPolygons);
    
    void  recalSegVertices(ofxTileMap* dynamicMap, Robot& scout, ofVec2f&target);
    
    bool  isIncircle(KERNEL::Position center, int radius, KERNEL::Position node);
    
    //--To classify vertices of boarder polygons of explored areas
    //--by analyzing the 8-connected points.
    unsigned char evaluateEdgePoint(ofxTileMap* dynamicMap, KERNEL::Position& vertex);
    
    bool lineInterCircle(const KERNEL::Position ptStart, const KERNEL::Position ptEnd, const KERNEL::Position ptCenter,
                         const float Radius, KERNEL::Position &ptInter1, KERNEL::Position &ptInter2);
    
    bool isReachable(std::set<esPolygon*> polygons, KERNEL::Position p1, KERNEL::Position p2);
    
    
    NodeVec visitNodes;
    
    std::vector< KERNEL::DoubleList<edgeNode> > proVertexSet;
    
    //-----------------debugging Log---------------
    std::fstream logFile;
    
};

#endif
