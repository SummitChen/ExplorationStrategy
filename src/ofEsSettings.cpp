//
//  ofEsSettings.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 21/07/2014.
//
//

#include "ofEsSettings.h"

ofEsSettings::ofEsSettings(){
    reset();
}

ofEsSettings::~ofEsSettings(){
    reset();
}

void ofEsSettings::reset(){
    configurationContainer.clear();
}

void ofEsSettings::load(string pathName){
    
    if ( XML.load(pathName)) {
        
        if ( XML.exists("CONFIGURATION")) {
            
            if( XML.setTo("CONFIGURATION[0]")){
                
                do{
                    esConfiguration configIt;
                    
                    configIt.playModel = XML.getValue<int>("PLAYMODEL");
                    configIt.duration  = XML.getValue<long unsigned int>("DURATION");
                    configIt.logPath   = XML.getValue<string>("LOGPATH");
                    configIt.mapPath   = XML.getValue<string>("MAP");
                    configIt.strategy  = XML.getValue<string>("STRATEGY");
                    configIt.sight     = XML.getValue<int>("SIGHT");
                    configIt.speed     = XML.getValue<float>("SPEED");
                    
                    unsigned int repeatTime = 1;
                    
                    for (unsigned int i = 0; i < repeatTime; ++i) {
                        configurationContainer.push_back(configIt);
                    }
                    
                }while(XML.setToSibling());
                
                XML.setToParent();
            }
        }
    }
}

std::vector<esConfiguration>& ofEsSettings::getConfigurationSet(){
    return configurationContainer;
}