//
//  exBotModel.h
//  ExplorationStrategy
//
//  Created by Chen Si on 12/06/2014.
//
//

#include "ofxMapRouting.h"
#include "ofxTileMap.h"
#include "ofxTimeMeasurements.h"
#include "Robot.h"
#include "ofEsLog.h"
#include "esModel.h"
#include "esStrategy.h"


#ifndef ExplorationStrategy_exBotModel_h
#define ExplorationStrategy_exBotModel_h

class esBotModel : public esModel{

public:
    
    virtual void       initialize(ofXml* settings);
    virtual void       initialize(esConfiguration& config);
    
    virtual void       updateStatus();
    virtual void       rendering();
    
    void               saveLog();
    
    //singleton pattern
    static esBotModel* getInstance();
    static void        destroyInstance();
    
    
protected:
    
    virtual void       selectNextPos();
    virtual void       initialize();
    
    void               calSegPerimeter();
    
    static esBotModel* instance;

    esStrategy*        exploreStrategy;
    unsigned long long timeDuration;

};

#endif
