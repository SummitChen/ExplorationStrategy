//
//  esBotModel.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#include "esBotModel.h"
#include "esMapAnalysis.h"
#include "esStrRandom.h"
#include "esStrGridbased.h"
#include "esStrFrontierbased.h"
#include "esStrFBMC.h"

esBotModel* esBotModel::instance = NULL;

esBotModel::esBotModel(){}

esBotModel::~esBotModel(){
    if (exploreStrategy != NULL) {
        delete exploreStrategy;
    }
}

esBotModel* esBotModel::getInstance(){
    
    if ( instance == NULL) {
        instance = new esBotModel();
    }
    
    return instance;
}

void esBotModel::destroyInstance(){
    
    esMapAnalysis::getInstance()->clearData();
    esBotModel::getInstance()->clearData();
    if ( instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

void esBotModel::initialize(ofXml* settings){
    
    loadSettings(settings);
    originAccout = 0;
    readOriginFile(mapPath);
    initialize();
    
}

void esBotModel::clearData(){
    if ( exploreStrategy != NULL) {
        delete exploreStrategy;
        exploreStrategy = NULL;
    }
    
    if ( map != NULL) {
        delete map;
        map = NULL;
    }
    
    if ( gridMap != NULL) {
        delete gridMap;
        gridMap = NULL;
    }
    
    if (routing != NULL) {
        delete routing;
        routing = NULL;
    }
    
    esMapAnalysis::getInstance()->clearData();
}

void esBotModel::initialize(){
    
    esModel::initialize();
    
    if ( strategy == "Random") {
        exploreStrategy = new esStrRandom();
    }else if( strategy == "Gridbased"){
        exploreStrategy = new esStrGridbased();
    }else if( strategy == "Frontierbased"){
        exploreStrategy = new esStrFrontierbased();
    }else if( strategy == "FBMC"){
        exploreStrategy = new esStrFBMC();
    }else{
        exploreStrategy = new esStrRandom();
    }
    
    exploreStrategy->setAlgorithmConfig(algorithmConfig);
    
    map = new ofxTileMap(mapPath);
    gridMap = new ofxTileMap(map->getWidth(), map->getHeight());
    routing = new ofxMapRouting( gridMap, false );
    
    ofSetWindowShape( map->getWidth(), map->getHeight());
    ofSetWindowPosition(12, 12);
    
    maxIterations = MAXNUM;
    connect8 = true;
    travelDistance = 0.0;
    timeDuration = 0;
    
    origin.x = originVec[originAccout].pos.x;
	origin.y = originVec[originAccout].pos.y;
    
    while ( map->getTileSafe( origin.x, origin.y) != WALKABLE) {
        origin.x = rand() % map->getWidth();
        origin.y = rand() % map->getHeight();
    }
    
    scout = Robot(origin, speed, sight);
    resetOrigin = true;
    
    beginTime = ofGetElapsedTimeMicros();
    //-Log
    syncLog.setMapName(mapPath);
    syncLog.setStrategyName(strategy);
    syncLog.setBeginTime(ofToString(beginTime));
    //----
    TIME_SAMPLE_START("exCalcPath");
    
    updateMapStatus();
    selectNextPos();
    
    //analyze Map
    esMapAnalysis::getInstance()->analyzeMap(map, gridMap);
    calSegPerimeter();
}

void esBotModel::initialize(esConfiguration& config){
    
    loadSettings(config);
    originAccout = 0;
    readOriginFile(mapPath);
    initialize();
    
}
void esBotModel::updateStatus(){
    
    TIME_SAMPLE_START("exCalcPath");
    
    routing->set8Connected(connect8);
    
    updateMapStatus();
    
    if ( resetOrigin || resetTarget) {
        
        origin = scout.getPosition();
        res.clear();
        TIME_SAMPLE_START("calcRoute");
        routing->calcRoute( origin.x, origin.y, target.x, target.y, res, maxIterations);
        //-log-
        syncLog.addPathPoint(target);
        //
        travelDistance += routing->getLastSolutionNumSteps();
        
        TIME_SAMPLE_STOP("calcRoute");
        
        resetTarget = false;
        resetOrigin = false;
        
        if ( res.size() == 0) {
            printf("A* failed in planning path from (%f, %f), to (%f, %f)\n", origin.x, origin.y, target.x, target.y);
            if ( gridMap->getTileSafe((int)target.x, (int)target.y) != WALKABLE){
                printf("target is not walkable!\n");
            }
        }else{
            TIME_SAMPLE_START("travelTimeEach");
            travelTimeEach = 0.0;
        }
    }else{
        
        travelTimeEach = TIME_SAMPLE_PAUSE("travelTimeEach");
        travelTimeEach /= 1000000.0;
        
        if ( (travelTimeEach * speed) < res.size() ) {
            if ((travelTimeEach * speed) >= 1) {
                scout.setPosition(ofVec2f(res.at(res.size() - (int)(travelTimeEach * speed)).x, res.at(res.size() - (int)(travelTimeEach * speed)).y));
            }
        }else if(goalAchieved){
            selectNextPos();
            goalAchieved = false;
        }else{
            scout.setPosition(ofVec2f(res[0].x, res[0].y));
            goalAchieved = true;
        }
        /*
         if( res.size() > 1) {
         scout.setPosition(ofVec2f(res.at(res.size()-1).x, res.at(res.size()-1).y));
         res.erase(res.end());
         }else{
         selectNextPos();
         }
         */
        
    }
    
    
    if ( (abs(scout.x() - target.x) <= 10.0
          && abs(scout.y() - target.y) <= 10.0) || res.size() == 0){
        selectNextPos();
    }
    
    TIME_SAMPLE_STOP("exCalcPath");
    
    //unsigned long long endTimeDuration = ofGetElapsedTimeMicros();
    
    timeDuration = (ofGetElapsedTimeMicros() - beginTime)/1000.0;
    
    //timeDuration += (endTimeDuration - startSimulation)/1000.0f;
    
    if ( timeDuration >= duration
        || ( gridExplorationPercentage >= 99.5 &&
            segExploratinoPercentage >= 99.5 &&
            resourceExplorationPercentage >= 99.5)) {
            saveLog();
            originAccout++;
            if (originAccout < originVec.size()) {
                clearData();
                initialize();
            }else{
                simulationEnd = true;
            }
        }
}

void esBotModel::selectNextPos(){
    
    exploreStrategy->calCandidatePosition(map, gridMap, resetTarget, target, scout, routing);
    calExploredPerimeter();
}

void esBotModel::saveLog(){
    
    TIME_SAMPLE_STOP("exCalcPath");
    
    //timeDuration += TIME_SAMPLE_GET_LAST_DURATION("exCalcPath");
    timeDuration = (ofGetElapsedTimeMicros() - beginTime)/1000.0;
    //-Log------
    syncLog.setEndTime(ofToString(ofGetElapsedTimeMicros()));
    syncLog.setDuration(timeDuration);
    syncLog.setGridMappingPercent(gridExplorationPercentage);
    syncLog.setFeatureMappingPercent(resourceExplorationPercentage);
    syncLog.setSegmentMappingPercent(segExploratinoPercentage);
    syncLog.setTravelDistance(travelDistance);
    //syncLog.save(ofToString(ofGetElapsedTimeMicros()) + "log.XML");
    
    unsigned int found = logPath.find_last_of(".");
    string transLogPath = logPath.substr(0, found) + "_" + ofToString(originVec[originAccout].originNumber) + ".XML";
    
    syncLog.save(transLogPath);
    //
}

void esBotModel::rendering(){
    esModel::rendering();
    //esMapAnalysis::getInstance()->draw();
    exploreStrategy->debugDraw();
}

void esBotModel::calSegPerimeter(){
    
    segPerimeter = 0.0;
    
    std::set<esPolygon*> fixedPolygons = esMapAnalysis::getInstance()->getUnwalkablePolygons();
    
    std::set<esPolygon*>::iterator it = fixedPolygons.begin();
    std::set<esPolygon*>::const_iterator end = fixedPolygons.end();
    
    //int acount = 0;
    for (; it != end; ++ it) {
        segPerimeter += calPolygonPerimeter(*(*it));
        //acount ++;
    }
    
    //printf("Number of polygons is %d \n", acount);
}