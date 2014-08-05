//
//  esStrGridbased.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 19/07/2014.
//
//

#include "esStrGridbased.h"
#include <map>
#include <cmath>
#include "esMapAnalysis.h"

//#define PI 3.1415926



void esStrGridbased::calCandidatePosition(ofxTileMap* solidMap, ofxTileMap* dynamicMap, bool& resetTarget, ofVec2f& target, Robot& scout, ofxMapRouting* routing){

    esMapAnalysis::getInstance()->analyzeExploredArea(dynamicMap);
    std::map<double, ofVec2f> candidatePosMap;
    std::vector<pos2>    res;
    int                maxIterations;
    int                travelDistance = 0;
    double             sumEvaluation = 0.0;
    double             costComponent = 0.0;
    double             tileExploredUtility = 0.0;
    double             SegExploredUtility = 0.0;
    double             FeaExploredUtility = 0.0;
    double             utitlyComponent = 0.0;
    
    float              alpha = 0.4;
    float              beta  = 0.6;
    KERNEL::Position   scoutLocation = KERNEL::Position(scout.x(), scout.y());
    bool               repeatFlag = false;
    
    for (unsigned int i = 0; i <= 360; i += 30) {
        
        int x = (int)(sin(i / PI) * ( scout.getSight() - scout.getRadius()) + scout.x());
        int y = (int)(cos(i / PI) * ( scout.getSight() - scout.getRadius()) + scout.y());
        
        if ( solidMap->getTileSafe(x, y) == WALKABLE) {
            
            for ( unsigned int j = 0; j < visitNodes.size(); ++ j) {
                if ( x == visitNodes[j].pos.x
                    && y == visitNodes[j].pos.y) {
                    repeatFlag = true;
                    break;
                }
            }
            
            if ( repeatFlag ) {
                repeatFlag = false;
                continue;
            }
            
            if ( x < 0 || x > dynamicMap->getWidth()
                || y < 0 || y > dynamicMap->getHeight()) {
                continue;
            }

            //cost facilities
            
            ofVec2f candidatePos(x, y);
            
            travelDistance = scoutLocation.getDistance(KERNEL::Position(x, y));
            costComponent = exp( scout.getRadius() - travelDistance);
            
            tileExploredUtility = calculateTilePercentage( dynamicMap, scout, candidatePos);
            SegExploredUtility = calculateSegPercentage(dynamicMap, scout, candidatePos);
            FeaExploredUtility = calculateFeaPercentage(dynamicMap, scout, candidatePos);
            
            utitlyComponent = 0.4 * tileExploredUtility + 0.4 * SegExploredUtility + 0.2 * FeaExploredUtility;
            
            sumEvaluation = alpha * costComponent + beta * utitlyComponent;
            //
            sumEvaluation = utitlyComponent;
            sumEvaluation = -sumEvaluation;
            
            candidatePosMap.insert(std::pair<double, ofVec2f>( sumEvaluation, candidatePos));
            
        }
    }
    
    if ( candidatePosMap.size() > 0) {
        target.x = candidatePosMap.begin()->second.x;
        target.y = candidatePosMap.begin()->second.y;
    }else{
        
        int next_pos_x = rand() % dynamicMap->getWidth();
        int next_pos_y = rand() % dynamicMap->getHeight();
        
        while (dynamicMap->getTileSafe( next_pos_x, next_pos_y) != WALKABLE) {
            next_pos_x = rand() % dynamicMap->getWidth();
            next_pos_y = rand() % dynamicMap->getHeight();
        }
        
        target.x = next_pos_x;
        target.y = next_pos_y;
    }
    
    //target = ofVec2f((float)resoultMap.begin()->second.x, (float)resoultMap.begin()->second.y);
    esNode nextTarget;
    nextTarget.pos = KERNEL::Position(target.x, target.y);
    nextTarget.visitFlag = true;
    visitNodes.push_back(nextTarget);
    
    resetTarget = true;

}