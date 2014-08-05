//
//  ofEsLog.h
//  ExplorationStrategy
//
//  Created by Chen Si on 4/06/2014.
//
//

#ifndef ExplorationStrategy_ofEsLog_h
#define ExplorationStrategy_ofEsLog_h

#include "ofMain.h"


class ofEsLog {

public:
    
    ofEsLog();
    ~ofEsLog();
    
    void save(string fileName);
    void reset();
    void load(string fileName);
    
    //Attributes
    void setBeginTime(string time);
    string getBeginTime();
    
    void setEndTime(string time);
    string getEndTime();
    
    void setDuration(float time);
    float getDuration();
    
    void setGridMappingPercent(float percentage);
    float getGridMappingPercent();
    
    void setSegmentMappingPercent(float percentage);
    float getSegmentMappingPercent();
    
    void setFeatureMappingPercent(float percentage);
    float getFeatureMappingPercent();
    
    void setMapName(string name);
    string getMapName();
    
    void setStrategyName(string name);
    string getStrategyName();
    
    void setPlayerName(string name);
    string getPlayerName();
    
    void setTravelDistance(float distance);
    float  getTravelDistance();
    
    void setExploredPath(const std::vector<ofVec2f>& vecpath);
    std::vector<ofVec2f>& getExplorationPath();
    
    void addPathPoint(ofVec2f point);
    
protected:
    
    string beginTime;
    string endTime;
    float  duration;
    float  travelDistance;
    
    float  gridMappingPercent;
    float  segmentMappingPercent;
    float  featureMappingPercent;
    
    string mapName;
    string strategyName;
    string playerName;
    
    std::vector<ofVec2f> explorePath;
    
    ofXml XML;
    
};

#endif
