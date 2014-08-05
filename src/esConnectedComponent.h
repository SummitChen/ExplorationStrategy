//
//  esConnectedComponent.h
//  ExplorationStrategy
//
//  Created by Chen Si on 4/07/2014.
//
//

#ifndef ExplorationStrategy_esConnectedComponent_h
#define ExplorationStrategy_esConnectedComponent_h

#include "ofVec2f.h"

class esConnectedComponent{

public:
    esConnectedComponent(int id, bool walkable);
   
    bool                 isWalkable(void) const;
    void                 setWalkable(bool walkability);
    
    int                  getID(void) const;
    
    ofVec2f              topLeft()   const;
    void                 setTopLeft(ofVec2f topLeftTile);

private:
    ofVec2f              _topLeftTile;
    bool                 _walkability;
    int                  _id;
    
};



#endif
