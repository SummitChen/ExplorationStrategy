//
//  ofEsLog.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 4/06/2014.
//
//

#include "ofEsLog.h"

ofEsLog::ofEsLog(){

    reset();
}

ofEsLog::~ofEsLog(){
  
    reset();
}
void ofEsLog::load(string pathName){

    if ( XML.load(pathName)) {
      
        if ( XML.exists("//BEGINTIME")) {
            beginTime = XML.getValue<string>("//BEGINTIME");
        }
        
        if ( XML.exists("//ENDTIME")) {
            endTime = XML.getValue<string>("//ENDTIME");
        }
        
        if ( XML.exists("//DURATION")) {
            duration = XML.getValue<float>("//DURATION");
        }
        
        if ( XML.exists("//GRIDMAPPINGPERCENT")) {
            gridMappingPercent = XML.getValue<float>("//GRIDMAPPINGPERCENT");
        }
        
        if ( XML.exists("//SEGMENTMAPPINGPERCENT")) {
            segmentMappingPercent = XML.getValue<float>("//SEGMENTMAPPINGPERCENT");
        }
        
        if ( XML.exists("//FEATUREMAPPINGPERCENT")) {
            featureMappingPercent = XML.getValue<float>("//FEATUREMAPPINGPERCENT");
        }
        
        if ( XML.exists("//MAP")) {
            mapName = XML.getValue<string>("//MAP");
        }
        
        if ( XML.exists("//STRATEGY")) {
            strategyName = XML.getValue<string>("//STRATEGY");
        }
        
        if ( XML.exists("//PLAYER")) {
            playerName = XML.getValue<string>("//PLAYER");
        }
        
        if ( XML.exists("PATH")) {
            
            XML.setTo("PATH[0]");
            
            do{
                if (XML.getName() == "PATH" && XML.setTo("PT[0]")) {
                    
                    do{
                        int x = XML.getValue<int>("X");
                        int y = XML.getValue<int>("Y");
                        
                        ofVec2f v(x, y);
                        
                        explorePath.push_back(v);
                    }while(XML.setToSibling());
                    
                    XML.setToParent();
                }
                
            }while(XML.setToSibling());
        }
        
    }
}

void ofEsLog::save(string fileName){

    XML.clear();
    
    XML.addChild("EXPLORATION");
    XML.setTo("//EXPLORATION");
    
    XML.addChild("TITLLE");
    XML.setTo("//TITLLE");
    
    XML.addChild("TIME");
    XML.setTo("//TIME");
    
    XML.addChild("BEGINTIME");
    XML.setValue("//BEGINTIME", ofToString(beginTime));
    
    XML.addChild("ENDTIME");
    XML.setValue("//ENDTIME", ofToString(endTime));
    
    XML.addChild("DURATION");
    XML.setValue("//DURATION", ofToString(duration));
    
    XML.setTo("//TITLLE");
    XML.addChild("MAPPINGPERCENTAGE");
    XML.setTo("//MAPPINGPERCENTAGE");
    
    XML.addChild("GRIDMAPPINGPERCENT");
    XML.setValue("//GRIDMAPPINGPERCENT", ofToString(gridMappingPercent));
    
    XML.addChild("SEGMENTMAPPINGPERCENT");
    XML.setValue("//SEGMENTMAPPINGPERCENT", ofToString(segmentMappingPercent));
    
    XML.addChild("FEATUREMAPPINGPERCENT");
    XML.setValue("//FEATUREMAPPINGPERCENT", ofToString(featureMappingPercent));
    
    XML.setTo("//TITLLE");
    
    XML.addChild("TRAVELDISTANCE");
    XML.setValue("//TRAVELDISTANCE", ofToString(travelDistance));
    
    XML.addChild("MAP");
    XML.setValue("//MAP", ofToString(mapName));
    
    XML.addChild("STRATEGY");
    XML.setValue("//STRATEGY", ofToString(strategyName));
    
    XML.addChild("PLAYER");
    XML.setValue("//PLAYER", ofToString(playerName));
    
    XML.setTo("//EXPLORATION");
    XML.addChild("PATH");
    XML.setTo("PATH[0]");
    
    for ( int i = 0; i < explorePath.size(); i++) {
        ofXml point;
        point.addChild("PT");
        point.setTo("PT");
        
        point.addValue("X", explorePath[i].x);
        point.addValue("Y", explorePath[i].y);
        
        XML.addXml(point);
    }
    
    XML.save(fileName);
    
}

void ofEsLog::reset(){

    XML.clear();
    
    beginTime = "";
    endTime = "";
    duration = 0.0;
    
    gridMappingPercent = 0.0;
    segmentMappingPercent = 0.0;
    featureMappingPercent = 0.0;
    
    mapName = "";
    strategyName = "";
    playerName = "";
    
    explorePath.clear();
}

void ofEsLog::setBeginTime(string time){
    beginTime = time;
}

string ofEsLog::getBeginTime(){
    return beginTime;
}

void ofEsLog::setEndTime(string time) {
    endTime = time;
}

string ofEsLog::getEndTime(){
    return endTime;
}

void ofEsLog::setDuration(float time){
    duration = time;
}

float ofEsLog::getDuration(){
    return duration;
}

void ofEsLog::setGridMappingPercent(float percentage){
    gridMappingPercent = percentage;
}

float ofEsLog::getGridMappingPercent(){
    return gridMappingPercent;
}

void ofEsLog::setSegmentMappingPercent(float percentage){
    segmentMappingPercent = percentage;
}

float ofEsLog::getSegmentMappingPercent(){
    return segmentMappingPercent;
}

void ofEsLog::setFeatureMappingPercent(float percentage){
    featureMappingPercent = percentage;
}

float ofEsLog::getFeatureMappingPercent(){
    return featureMappingPercent;
}

void ofEsLog::setMapName(string name){
    mapName = name;
}

string ofEsLog::getMapName(){
    return mapName;
}

void ofEsLog::setStrategyName(string name){
    strategyName = name;
}

string ofEsLog::getStrategyName(){
    return strategyName;
}

void ofEsLog::setPlayerName(string name){
    playerName = name;
}

string ofEsLog::getPlayerName(){
    return playerName;
}

void ofEsLog::setExploredPath(const std::vector<ofVec2f> &vecpath){
    explorePath = vecpath;
}

std::vector<ofVec2f>& ofEsLog::getExplorationPath(){
    return explorePath;
}

void ofEsLog::addPathPoint(ofVec2f point){
    explorePath.push_back(point);
}

void ofEsLog::setTravelDistance(float distance){
    travelDistance = distance;
}

float ofEsLog::getTravelDistance(){
    return travelDistance;
}
