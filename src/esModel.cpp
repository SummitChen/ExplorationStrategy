//
//  esModel.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#include "esModel.h"
#include "esStrategy.h"
#include "esMapAnalysis.h"

esModel::~esModel(){
    
    printf("call esModel destructor!\n");
    
    if ( map != NULL) {
        delete map;
        map = NULL;
    }
    
    if ( gridMap != NULL) {
        delete gridMap;
        gridMap = NULL;
    }
    
    if ( routing != NULL) {
        delete routing;
        routing = NULL;
    }
}

ofxTileMap* esModel::getStaticMap(){
    return map;
}

ofxTileMap* esModel::getDynamicMap(){
    return gridMap;
}

ofxMapRouting* esModel::getRouting(){
    return routing;
}

std::vector<pos2>& esModel::getPath(){
    return res;
}

int esModel::getMaxIterations(){
    return maxIterations;
}

bool esModel::getConnect8(){
    return connect8;
}

const ofVec2f& esModel::getOrigin(){
    return origin;
}

const ofVec2f& esModel::getTarget(){
    return target;
}

void  esModel::setStaticMap(ofxTileMap* staticMap){
    map = staticMap;
}

void esModel::setDynamicMap(ofxTileMap *dynamicMap){
    gridMap = dynamicMap;
}

void esModel::setRouting(ofxMapRouting *pathRout){
    routing = pathRout;
}

void esModel::setMaxIteration(int maxIter){
    maxIterations = maxIter;
}

void esModel::setConnect8(bool con){
    connect8 = con;
}

void esModel::setOrigin(const ofVec2f &ori){
    origin = ori;
}

void esModel::setTarget(const ofVec2f &tar){
    target = tar;
}

void esModel::updateMapStatus(){
    //update grid-map
    int sight_cone = scout.getSight();
    for ( int i = -sight_cone; i <= sight_cone; ++i) {
        for ( int j = -sight_cone; j <= sight_cone; ++j) {
            if ( pow(float(i), 2) + pow(float(j), 2) <= pow(float(sight_cone), 2)) {
                gridMap->setTileSafe(scout.x() + i, scout.y() + j, map->getTileSafe(scout.x() + i, scout.y() + j));
            }
        }
    }
    gridMap->updateMapImage();
}

void esModel::drawMap(){
    
    ofSetColor(255);
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    if ( map != NULL &&
        gridMap != NULL) {
        map->draw();
        gridMap->draw();
    }
    
    ofDisableAlphaBlending();
    
}

void esModel::drawRobot(){
    scout.draw();
}

void esModel::drawPath(){
    //solution
	ofMesh m;
	m.setMode(OF_PRIMITIVE_POINTS);
	for (int i = 0; i < res.size(); i++){
		pos2 p = res[i];
		m.addVertex( ofVec2f(p.x, p.y) );
	}
	ofSetColor(255,0,0);
	m.draw();
    
    drawRobot();
    
	ofNoFill();
	ofSetColor(255, 0, 0, 128);
	ofCircle(target.x, target.y, 10);
	ofCircle(target.x, target.y, 3);
	ofFill();
	
	//ofSetColor(255);
	ofDrawBitmapString("destination", target.x + 12, target.y + 3);
    m.clear();
}

void esModel::drawInformation(){
    
    //exec times
	ofPushMatrix();
    ofTranslate(ofGetWidth() - 250, 0);
    ofSetColor(0,0,0, 200);
    //ofRect(20, 80, 250, 80);
    ofSetColor(255);
    TIME_SAMPLE_SET_DRAW_LOCATION(TIME_MEASUREMENTS_TOP_LEFT);
    //ofxTimeMeasurements::instance()->draw(20, 80);
    ofPopMatrix();
    
	
	ofSetColor(255, 0, 0);
	int xoff = -300;
	int yoff = -110;
	int lineH = -15;

	ofDrawBitmapString( "solution num steps: " + ofToString(routing->getLastSolutionNumSteps()) , ofGetWidth()  + xoff, ofGetHeight() + yoff); yoff -= lineH;
	//ofDrawBitmapString( "solution path cost: " + ofToString(routing->getLastSolutionPathCost(), 1) , ofGetWidth() + xoff, ofGetHeight() - 30); yoff -= lineH;
    
    ofDrawBitmapString( "travelDistance: " + ofToString(travelDistance, 1) , ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= lineH;
    
    if ( map->accFreeTile == 0) {
        
        ofDrawBitmapString( "Exploration Percentage: 0 %" , ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= lineH;
        
    }else{
        
        gridExplorationPercentage = gridMap->accFreeTile / ( map->accFreeTile * 1.0 ) * 100;
        
        if ( gridExplorationPercentage >= 100.0) {
            gridExplorationPercentage = 100.0;
        }
        
        ofDrawBitmapString( "Exploration Percentage: " + ofToString( gridExplorationPercentage, 1) + "%\nstatic :" + ofToString( map->accFreeTile, 1) + " explored " + ofToString(gridMap->accFreeTile, 1),  ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= 2 * lineH;
    }
    
    if ( map->resourceTile == 0) {
        
        ofDrawBitmapString( "Resource Detection Percentage: 0 %" , ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= lineH;
        
    }else{
        
        resourceExplorationPercentage = gridMap->resourceTile / ( map->resourceTile * 1.0 ) * 100;
        
        if ( resourceExplorationPercentage >= 100.0) {
            resourceExplorationPercentage = 100.0;
        }
        
        ofDrawBitmapString( "Resource Detection Percentage: " + ofToString( resourceExplorationPercentage, 1) + "%\nstatic :" + ofToString( map->resourceTile, 1) + " explored " + ofToString(gridMap->resourceTile, 1),  ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= 2 * lineH;
    }
    
    if ( segPerimeter < 1.0) {
        
        ofDrawBitmapString( "Segment Detection Percentage: 0 %" , ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= lineH;
        
    }else{
        
        segExploratinoPercentage = exploredPerimeter / segPerimeter * 100;
        
        if ( segExploratinoPercentage >= 100.0) {
            segExploratinoPercentage = 100.0;
        }
        
        ofDrawBitmapString( "Segment Detection Percentage: " + ofToString( segExploratinoPercentage, 1) + "%\nstatic :" + ofToString( segPerimeter, 1) + " explored " + ofToString(exploredPerimeter, 1),  ofGetWidth() + xoff, ofGetHeight() + yoff); yoff -= 2 * lineH;
    }
}

void esModel::rendering(){
    
    drawMap();
    drawPath();
    drawInformation();
    
}

void esModel::loadSettings(ofXml *settings){

    if(settings->exists("//LOGPATH")){
        logPath = settings->getValue<string>("//LOGPATH");
    }
    
    if(settings->exists("//MAP")){
        mapPath = settings->getValue<string>("//MAP");
    }
    
    if (settings->exists("//STRATEGY")) {
        strategy = settings->getValue<string>("//STRATEGY");
    }
    
    if (settings->exists("//SIGHT")) {
        sight = settings->getValue<int>("//SIGHT");
    }
    
    if (settings->exists("//SPEED")) {
        speed = settings->getValue<float>("//SPEED");
    }
    
    if (settings->exists("//DURATION")) {
        duration = settings->getValue<unsigned long long>("//DURATION");
    }
}

void esModel::loadSettings(esConfiguration& config){
    
    logPath = config.logPath;
    mapPath = config.mapPath;
    strategy = config.strategy;
    sight = config.sight;
    speed = config.speed;
    duration = config.duration;
    algorithmConfig = config.configHFrontier;
    
}

bool esModel::isSimulationEnd(){
    return simulationEnd;
}

void esModel::initialize(){
    simulationEnd = false;
    travelTimeEach = 0.0;
    goalAchieved = false;
}

float esModel::calPolygonPerimeter(esPolygon p){
    
    float polyPerimeter = 0.0;
    KERNEL::Position iter;
    KERNEL::Position trail;
    
    for (unsigned int i = 0; i < p.pointSet.size(); ++ i) {
        iter = p.pointSet[i];
        if ( i < p.pointSet.size() - 1) {
            trail = p.pointSet[i+1];
        }else{
            trail = p.pointSet[0];
        }
        
        polyPerimeter += iter.getDistance(trail);
    }
    
    for (unsigned int j = 0; j < p.getHoles().size(); ++ j) {
        polyPerimeter += calPolygonPerimeter(p.getHoles()[j]);
    }
    
    return polyPerimeter;
    
}

void esModel::calExploredPerimeter(){
    
    exploredPerimeter = 0.0;
    
    std::vector<std::vector<edgeNode> > proVertexVec;
    std::vector<edgeNode> calculatedVertex;
    
    std::set<esPolygon*> exploredPolygon = esMapAnalysis::getInstance()->getExploredPolygons();
    
    std::set<esPolygon*>::iterator it = exploredPolygon.begin();
    std::set<esPolygon*>::iterator end = exploredPolygon.end();
    
    for (; it != end; ++ it) {
        
        std::vector<edgeNode>  vertexVec;
        
        for ( unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
            edgeNode node;
            node.pos = (*it)->pointSet[i];
            node.explorationFlag = evaluateEdgePoint(gridMap, node.pos);
            vertexVec.push_back(node);
        }
        
        proVertexVec.push_back(vertexVec);
        
        for ( unsigned int j = 0; j < (*it)->getHoles().size(); ++ j) {
            
            std::vector<edgeNode> holeVertexVec;
            
            for ( unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); ++ k) {
                
                edgeNode hNode;
                hNode.pos = (*it)->getHoles()[j].pointSet[k];
                hNode.explorationFlag = evaluateEdgePoint(gridMap, hNode.pos);
                holeVertexVec.push_back(hNode);
            }
            
            proVertexVec.push_back(holeVertexVec);
        }
    }
    
    edgeNode iter;
    edgeNode trail;
    
    for ( unsigned int i = 0; i < proVertexVec.size(); ++ i) {
        for ( unsigned int j = 0; j < proVertexVec[i].size(); ++ j) {
            iter = proVertexVec[i][j];
            
            for ( unsigned int k = 0; k < calculatedVertex.size(); ++ k) {
                
            }
            if (j < proVertexVec[i].size() - 1) {
                trail = proVertexVec[i][j+1];
            }else{
                trail = proVertexVec[i][0];
            }
            
            if ( iter.explorationFlag == UN_WALKABLE
                && trail.explorationFlag == UN_WALKABLE) {
                exploredPerimeter += iter.pos.getDistance(trail.pos);
            }
        }
    }
}

unsigned char esModel::evaluateEdgePoint(ofxTileMap *dynamicMap, KERNEL::Position &vertex){
    int sumUnknown = 0;
    int sumWalkable = 0;
    int sumObstacle = 0;
    
    int width = dynamicMap->getWidth();
    int height = dynamicMap->getHeight();
    
    for (int i = vertex.x - 1; i <= vertex.x + 1; ++ i) {
        for ( int j = vertex.y - 1; j <= vertex.y + 1; ++j) {
            
            if ( i < 0 || i > width
                || j < 0 || j > height) {
                sumObstacle ++;
                continue;
            }
            
            switch (dynamicMap->getTileSafe(i, j)) {
                case UN_KNOWN:
                    sumUnknown ++;
                    break;
                case UN_WALKABLE:
                    sumObstacle ++;
                    break;
                case WALKABLE:
                    sumWalkable ++;
                    break;
                default:
                    sumObstacle ++;
                    break;
            }
        }
    }
    
    if ( sumObstacle == sumUnknown &&
        sumObstacle == 0) {
        return WALKABLE;
    }else if( sumObstacle != 0){
        return UN_WALKABLE;
    }else{
        return UN_KNOWN;
    }
}

void esModel::clearData(){
    esMapAnalysis::getInstance()->clearData();
}

void esModel::readOriginFile(string mapPath){

    unsigned int found = mapPath.find_last_of(".");
    string dataPath = mapPath.substr(0, found) + "_1.XML";
    
    ofXml XML;
    
    if ( XML.load(dataPath)) {
        
        if ( XML.exists("ORIGIN")) {
            
            XML.setTo("ORIGIN[0]");
            
            do{
                if (XML.getName() == "ORIGIN") {
                    
                    OriginStruct origin;
                    origin.pos.x = XML.getValue<int>("X");
                    origin.pos.y = XML.getValue<int>("Y");
                    origin.originNumber = XML.getValue<int>("SEQUENCE");
                    
                    originVec.push_back(origin);
                }
                
            }while(XML.setToSibling());
        }
        
    }
}