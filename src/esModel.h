//
//  esModel.h
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#ifndef ExplorationStrategy_esModel_h
#define ExplorationStrategy_esModel_h

#include "ofxMapRouting.h"
#include "ofxTileMap.h"
#include "ofxTimeMeasurements.h"
#include "Robot.h"
#include "ofEsLog.h"
#include "ofEssettings.h"
#include "esPolygon.h"

#define MAXNUM 65535000

class esModel{

public:

    virtual void       initialize(ofXml* settings){}
    virtual void       initialize(esConfiguration& config){}
    
    
    virtual void       updateStatus(){}
    virtual void       rendering();
    
    ofxTileMap*        getStaticMap();
    ofxTileMap*        getDynamicMap();
    ofxMapRouting*     getRouting();
    
    std::vector <pos2>&  getPath();
    int                getMaxIterations();
    bool               getConnect8();
    
    const ofVec2f&     getOrigin();
    const ofVec2f&     getTarget();
    
    bool               isSimulationEnd();
    
    
protected:
    
    virtual void       clearData();
    
    virtual void       selectNextPos(){}
    virtual void       updateMapStatus();
    
    virtual void       drawMap();
    virtual void       drawRobot();
    virtual void       drawPath();
    virtual void       drawInformation();
    
    void               setStaticMap(ofxTileMap* map);
    void               setDynamicMap(ofxTileMap* map);
    void               setRouting(ofxMapRouting* routing);
    
    void               setMaxIteration(int maxIter);
    void               setConnect8(bool con);
    
    void               setOrigin(const ofVec2f& origin);
    void               setTarget(const ofVec2f& target);
    
    virtual void       loadSettings(ofXml* settings);
    virtual void       loadSettings(esConfiguration& config);
    virtual void       initialize();
    
    float              calPolygonPerimeter(esPolygon p);
    void               calExploredPerimeter();
    
    //--To classify vertices of boarder polygons of explored areas
    //--by analyzing the 8-connected points.
    unsigned char evaluateEdgePoint(ofxTileMap* dynamicMap, KERNEL::Position& vertex);
    
    
//-----------------------------------------------------------
    
    //-Map
    ofxTileMap*        map;
    ofxTileMap*        gridMap;
    string             mapPath;
    //-Strategy
    string             strategy;
    unsigned long long duration;
    
    //-Path finding
    ofxMapRouting*     routing;
    std::vector <pos2>   res;
    int                maxIterations;
    bool               connect8;
    
    ofVec2f            origin;
    ofVec2f            target;
    
    bool               resetOrigin;
    bool               resetTarget;
    
    //- Log
    ofEsLog            syncLog;
    string             logPath;
    
    //- Scout unit
    Robot              scout;
    int                sight;
    float              speed;
    
    //- Measurement data
    float              gridExplorationPercentage;
    float              travelDistance;
    float              resourceExplorationPercentage;
    float              segPerimeter;
    float              exploredPerimeter;
    float              segExploratinoPercentage;
    
    
    bool               simulationEnd;

};

#endif
