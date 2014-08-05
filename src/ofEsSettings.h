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
