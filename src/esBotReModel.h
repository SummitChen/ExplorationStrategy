//
//  esBotReModel.h
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#ifndef ExplorationStrategy_esBotReModel_h
#define ExplorationStrategy_esBotReModel_h

#include "esModel.h"

class esBotReModel : public esModel{

public:
    
    
    virtual void         initialize(ofXml* settings);
    virtual void         initialize(esConfiguration& config);
    
    virtual void         updateStatus();
        
    //singleton pattern
    static esBotReModel* getInstance();
    static void          destroyInstance();
    
protected:
    
    virtual void         selectNextPos();
    virtual void         initialize();
    
    static esBotReModel* instance;
    
    int                  pathSize;
    int                  iterater;
    
};

#endif
