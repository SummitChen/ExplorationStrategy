//
//  esMapAnalysis.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 14/07/2014.
//
//

#include <stddef.h>
#include "esMapAnalysis.h"
#include "RectangleArray.h"
#include "esExtractPolygons.h"
#include "esConnectedComponent.h"

esMapAnalysis* esMapAnalysis::instance = NULL;

esMapAnalysis::esMapAnalysis(){

}

esMapAnalysis::~esMapAnalysis(){

}

esMapAnalysis* esMapAnalysis::getInstance(){
    if ( instance == NULL) {
        instance = new esMapAnalysis();
    }
    
    return instance;
}

void esMapAnalysis::deleteInstance(){
    
    instance->clearData();
    
    if ( instance != NULL) {
        delete instance;
        instance = NULL;
    }
}


void esMapAnalysis::analyzeMap(ofxTileMap* solidMap, ofxTileMap* dynamicMap){
    KERNEL::RectangleArray<esConnectedComponent*> getComponent;
    //KERNEL::RectangleArray<esConnectedComponent*> getComponentU;
    std::list<esConnectedComponent> components;
    //std::list<esConnectedComponent> componentsU;
    
    // Give find_connected_components the walkability data so it can compute the list of connected components,
    // and determine which component each tile belongs to
    if ( solidMap->map != NULL) {
        
        bool* walkbilityData = new bool[ solidMap->getWidth() * solidMap->getHeight()];
        //bool* unwalkbilityData = new bool[ solidMap->getWidth() * solidMap->getHeight()];
        
        for ( unsigned int i = 0; i < solidMap->getWidth(); i ++) {
            for (unsigned int j = 0; j < solidMap->getHeight(); j ++) {
                if ( solidMap->getTileSafe(i, j) == UN_WALKABLE) {
                    walkbilityData[i * solidMap->getHeight() + j] = true;
                    //unwalkbilityData[i * solidMap->getHeight() + j] = false;
                }else{
                    walkbilityData[i * solidMap->getHeight() + j] = false;
                    //unwalkbilityData[i * solidMap->getHeight() + j] = true;
                }
            }
        }
        
        KERNEL::RectangleArray<bool> walkabilityArray(solidMap->getWidth(), solidMap->getHeight(), walkbilityData);
        //KERNEL::RectangleArray<bool> unwalkabilityArray(solidMap->getWidth(), solidMap->getHeight(), unwalkbilityData);
        
        
        findConnectedComponents(walkabilityArray, getComponent, components);
        //findConnectedComponents(unwalkabilityArray, getComponentU, componentsU);

        
        std::vector<esPolygon> polygons;
        //std::vector<esPolygon> polygonsU;
        // Give extract_polygons the walkability data and connected components so it can compute the polygonal obstacles
        extractPolygons(walkabilityArray, components, polygons);
        //extractPolygons(unwalkabilityArray, componentsU, polygonsU);

        // Discard polygons that are too small
        for ( unsigned int p = 0; p < polygons.size(); ) {
            if (abs(polygons[p].getArea()) <= 20)
            {
                polygons.erase(polygons.begin() + p);
            }
            else
            {
                p++;
            }
        }
        
                
        // Save the remaining polygons
        for( unsigned int i = 0; i < polygons.size(); i++){
            unwalkablePolygons.insert(new esPolygon(polygons[i]));
        }
        
        //-------------debugging-------------------------------------------------
        debugmesh.clear();
        
        debugmesh.setMode(OF_PRIMITIVE_POINTS);
        
        //-------------------------debugging------------------------------
        std::set<esPolygon*>::iterator it = unwalkablePolygons.begin();
        std::set<esPolygon*>::const_iterator end = unwalkablePolygons.end();
        
        unsigned int account = 0;

        for (; it != end; ++it) {
            
            account++;
            for (unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
                debugmesh.addVertex(ofVec2f((*it)->pointSet[i].x, (*it)->pointSet[i].y));
            }
            if ((*it)->getHoles().size() != 0) {
                for (unsigned int j = 0; j < (*it)->getHoles().size(); ++j) {
                    for (unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); k ++) {
                        debugmesh.addVertex(ofVec2f((*it)->getHoles()[j].pointSet[k].x,
                                                    (*it)->getHoles()[j].pointSet[k].y));
                    }
                    account++;
                }
            }
        }
        
        //-clear-data--------------
        delete [] walkbilityData;
        
        //printf("Number of polygons is %u\n", account);

    }
}

void esMapAnalysis::draw(const esPolygon &polygon){
    ofSetPolyMode(OF_POLY_WINDING_NONZERO);
    ofBeginShape();
    for (unsigned int i = 0; i < polygon.pointSet.size(); i ++) {
        ofVertex(polygon.pointSet[i].x, polygon.pointSet[i].y);
    }
    for (unsigned int j = 0; j < polygon.holes.size(); j++) {
        draw(polygon.holes[j]);
    }
    ofEndShape();
}

void esMapAnalysis::draw(const std::set<esPolygon *> &polygons){
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(5.0);
    ofNoFill();
    std::set<esPolygon *>::iterator cursor = polygons.begin();
    std::set<esPolygon *>::const_iterator end = polygons.end();
    
    for (; cursor != end;  ++ cursor) {
        draw(*(*cursor));
        //break;
    }
    
    ofPopStyle();
}

void esMapAnalysis::draw(){
    //draw(unwalkablePolygons);
    //draw(exploredPolygons);
    //debugDraw();
}

void esMapAnalysis::debugDraw(){
 //To test the correction of walkablity
    ofPushStyle();
    ofSetColor(255, 255, 0);
    ofSetLineWidth(5.0);
    debugmesh.draw();
    ofPopStyle();
}

void esMapAnalysis::analyzeExploredArea(ofxTileMap *dynamicMap){
    clearPolygonSet(exploredPolygons);
    
    //-------------------debugging----------------------
    unsigned int size = exploredPolygons.size();
    
    KERNEL::RectangleArray<esConnectedComponent*> getComponent;
    std::list<esConnectedComponent> components;
    
    if ( dynamicMap != NULL) {
        bool* unexploredData = new bool[ dynamicMap->getWidth() * dynamicMap->getHeight()];
        
        for ( unsigned int i = 0; i < dynamicMap->getWidth(); i ++) {
            for (unsigned int j = 0; j < dynamicMap->getHeight(); j ++) {
                if ( dynamicMap->getTileSafe(i, j) == WALKABLE) {
                    unexploredData[i * dynamicMap->getHeight() + j] = false;
                }else{
                    unexploredData[i * dynamicMap->getHeight() + j] = true;
                }
            }
        }
        
        KERNEL::RectangleArray<bool> unexploredArray(dynamicMap->getWidth(), dynamicMap->getHeight(), unexploredData);
        
        findConnectedComponents(unexploredArray, getComponent, components);
        
        std::vector<esPolygon> polygons;

        extractPolygons(unexploredArray, components, polygons);
        
        // Discard polygons that are too small
        for ( unsigned int p = 0; p < polygons.size(); ) {
            if (abs(polygons[p].getArea()) <= 20)
            {
                polygons.erase(polygons.begin() + p);
            }
            else
            {
                p++;
            }
        }
        
        // Save the remaining polygons
        for( unsigned int i = 0; i < polygons.size(); i++){
            exploredPolygons.insert(new esPolygon(polygons[i]));
        }
        
        //-clear-data--------------
        delete [] unexploredData;
        
#if 0
        //------------debugging---------------------------------
        printf("Analyzed explored polygon size %lu \n", exploredPolygons.size());
        
        std::set<esPolygon*>::iterator pit = exploredPolygons.begin();
        std::set<esPolygon*>::iterator pend = exploredPolygons.end();
        
        for ( unsigned int i = 0; pit != pend; ++ pit, ++ i) {
            printf("Analyzed polygon %u size %lu \n", i, (*pit)->pointSet.size());
            if ( (*pit)->getHoles().size() != 0) {
                printf("Analyzed polygon %u holes size %lu \n", i, (*pit)->getHoles().size());
                for ( unsigned int j = 0; j < (*pit)->getHoles().size(); ++ j) {
                    printf("Analyzed polygon %u hole %u size %lu \n", i, j, (*pit)->getHoles()[j].pointSet.size());
                }
            }
        }
#endif
    }
}

void esMapAnalysis::clearPolygonSet(std::set<esPolygon *> &polygonSet){
    std::set<esPolygon*>::iterator it = polygonSet.begin();
    std::set<esPolygon*>::const_iterator end = polygonSet.end();
    for (; it != end; ++it) {
        if ((*it) != NULL) {
            delete (*it);
        }
    }
    
    polygonSet.clear();
}


std::set<esPolygon*>& esMapAnalysis::getExploredPolygons(){
    return exploredPolygons;
}

void esMapAnalysis::clearExploredPolygons(){
    clearPolygonSet(exploredPolygons);
    //debugMesh2.clear();
}

void esMapAnalysis::clearUnwalkablePolygons(){
    clearPolygonSet(unwalkablePolygons);
}

std::set<esPolygon*>& esMapAnalysis::getUnwalkablePolygons(){
    return unwalkablePolygons;
}

void esMapAnalysis::clearData(){
    clearExploredPolygons();
    clearUnwalkablePolygons();
    debugmesh.clear();
}