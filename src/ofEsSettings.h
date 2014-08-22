//
//  ofEsSettings.h
//  ExplorationStrategy
//
//  Created by Chen Si on 21/07/2014.
//
//

#ifndef ExplorationStrategy_ofEsSettings_h
#define ExplorationStrategy_ofEsSettings_h

#include "ofMain.h"

class algorithmConfig{
    
public:

    algorithmConfig(){
        iSegment = 0.4;
        iGrid = 0.4;
        iFeature = 0.2;
        alpha = 0.4;
        beta = 0.6;
    }
    
    ~algorithmConfig(){}
    
    void setSegment(float value){
        if( value > 1.0 || value < 0.0){
            iSegment = 0.4;
        }else{
            iSegment = value;
        }
    }
    float getSegment(){
        return iSegment;
    }
    
    void setGrid(float value){
        if( value > 1.0 || value < 0.0){
            iGrid = 0.4;
        }else{
            iGrid = value;
        }
    }
    float getGrid(){
        return iGrid;
    }
    
    void setFeature(float value){
        if( value > 1.0 || value < 0.0){
            iFeature = 0.2;
        }else{
            iFeature = value;
        }
    }
    float getFeature(){
        return iFeature;
    }
    
    void setAlpha(float value){
        if( value > 1.0 || value < 0.0){
            alpha = 0.4;
        }else{
            alpha = value;
        }
    }
    float getAlpha(){
        return alpha;
    }
    
    void setBeta(float value){
        if( value > 1.0 || value < 0.0){
            beta = 0.6;
        }else{
            beta = value;
        }
    }
    float getBeta(){
        return beta;
    }
    
private:
    float iSegment;
    float iGrid;
    float iFeature;
    float alpha;
    float beta;
};

class esConfiguration{

public:
    
    esConfiguration(){}
    ~esConfiguration(){}
    
    int                 playModel;
    long unsigned int   duration;
    
    string              logPath;
    string              mapPath;
    string              strategy;
    
    unsigned int        sight;
    float               speed;
    
    algorithmConfig     configHFrontier;
    
    
};

class ofEsSettings{

public:
    
    ofEsSettings();
    ~ofEsSettings();
    
    void load(string fileName);
    void reset();
    
    std::vector<esConfiguration>& getConfigurationSet();
    
private:
    
    ofXml XML;
    std::vector<esConfiguration> configurationContainer;
    
};

#endif
