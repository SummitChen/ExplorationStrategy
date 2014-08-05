//
//  esBotReModel.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#include "esBotReModel.h"

esBotReModel* esBotReModel::instance = NULL;

esBotReModel* esBotReModel::getInstance(){
    
    if ( instance == NULL) {
        instance = new esBotReModel();
    }
    
    return instance;
}

void esBotReModel::destroyInstance(){
    
    if ( instance != NULL){
        delete instance;
        instance = NULL;
    }
}


void esBotReModel::initialize( ofXml* settings){
    
    loadSettings(settings);
    initialize();
    
}

void esBotReModel::initialize(){
    
    esModel::initialize();
    
    syncLog.load(logPath);
    map = new ofxTileMap(syncLog.getMapName());
    gridMap = new ofxTileMap(map->getWidth(), map->getHeight());
    routing = new ofxMapRouting( map, false );
    
    ofSetWindowShape(map->getWidth(), map->getHeight());
    ofSetWindowPosition(12, 12);
	//setUIParams();
    
    maxIterations = MAXNUM;
    
    origin.x = map->getWidth()/2;
	origin.y = map->getHeight()/2 - 5;
    
    while ( map->getTileSafe( origin.x, origin.y) != WALKABLE) {
        origin.x = rand() % map->getWidth();
        origin.y = rand() % map->getHeight();
    }
    
    scout = Robot( origin, speed, sight);
    resetOrigin = true;
    
    updateMapStatus();
    
    if (syncLog.getExplorationPath().size() > 1) {
        pathSize = syncLog.getExplorationPath().size();
        iterater = 0;
    }
    
    selectNextPos();
}

void esBotReModel::initialize(esConfiguration& config){

    loadSettings(config);
    initialize();
    
}
void esBotReModel::updateStatus(){

    routing->set8Connected(connect8);
    
    updateMapStatus();
    
    if ( resetOrigin || resetTarget) {
        
        origin = scout.getPosition();
        res.clear();
        TIME_SAMPLE_START("calcRoute");
        routing->calcRoute( origin.x, origin.y, target.x, target.y, res, maxIterations);
        
        TIME_SAMPLE_STOP("calcRoute");
        
        resetOrigin = false;
        resetTarget = false;
        
    }else if( res.size() > 1) {
        scout.setPosition(ofVec2f(res.at(res.size()-1).x, res.at(res.size()-1).y));
        res.erase(res.end());
    }
    
    if ( abs(scout.x() - target.x) <= 10.0
        && abs(scout.y() - target.y) <= 10.0){
        selectNextPos();
    }
}

void esBotReModel::selectNextPos(){

    if ( iterater < pathSize) {
        target = syncLog.getExplorationPath()[iterater];
        resetTarget = true;
        iterater++;
    }else{
        simulationEnd = true;
    }

}