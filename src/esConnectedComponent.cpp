//
//  esConnectedComponent.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 4/07/2014.
//
//

#include "esConnectedComponent.h"

esConnectedComponent::esConnectedComponent(int id, bool walkable)
: _id(id),
_walkability(walkable),
_topLeftTile(10000, 10000){
    
}

bool esConnectedComponent::isWalkable(void) const{
    return _walkability;
}

void esConnectedComponent::setWalkable(bool walkability){
    this->_walkability = walkability;
}

int  esConnectedComponent::getID() const{
    return this->_id;
}

ofVec2f esConnectedComponent::topLeft() const{
    return this->_topLeftTile;
}

void esConnectedComponent::setTopLeft(ofVec2f topLeftTile){
    this->_topLeftTile = topLeftTile;
}