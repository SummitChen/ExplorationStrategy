//
//  esMapAnalysis.h
//  ExplorationStrategy
//
//  Created by Chen Si on 14/07/2014.
//
//

#ifndef ExplorationStrategy_esMapAnalysis_h
#define ExplorationStrategy_esMapAnalysis_h

#include <vector>
#include "esPolygon.h"
#include "ofxTileMap.h"
#include "RectangleArray.h"

class esMapAnalysis{

public:
    
    static esMapAnalysis* getInstance();
    static void           deleteInstance();
    
    void   analyzeMap( ofxTileMap* solidMap, ofxTileMap* dynamciMap);
    void   draw(const esPolygon& polygon);
    void   draw(const std::set<esPolygon*>& polygons);
    //Debug drawing
    void   analyzeExploredArea( ofxTileMap* dynamicMap);
    
    void   debugDraw();
    void   draw();
    
    std::set<esPolygon*>& getUnwalkablePolygons();
    std::set<esPolygon*>& getExploredPolygons();
    
    void   clearExploredPolygons();
    void   clearUnwalkablePolygons();
    void   clearData();
    
private:
    
    void   clearPolygonSet(std::set<esPolygon*>& polygonSet);
    static esMapAnalysis* instance;
    
    esMapAnalysis();
    ~esMapAnalysis();
    
    std::set<esPolygon*> unwalkablePolygons;
    std::set<esPolygon*> exploredPolygons;
    
    //debug parameters
    KERNEL::RectangleArray<bool> walkabletestArray;
    ofMesh debugmesh;
    
    
};
#endif
