//
//  esStrFBMC.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 29/07/2014.
//
//

#include "esStrFBMC.h"

#include "esMapAnalysis.h"

void esStrFBMC::calCandidatePosition(ofxTileMap *solidMap, ofxTileMap *dynamicMap, bool &resetTarget, ofVec2f &target, Robot &scout, ofxMapRouting *routing){
    
    exploredMap = dynamicMap;
    scoutBot = scout;
    
    esMapAnalysis::getInstance()->analyzeExploredArea(dynamicMap);
    
    KERNEL::Position scoutLocation = KERNEL::Position(scout.x(), scout.y());
    
    std::vector<KERNEL::Position> globleCanPos;
    std::vector<KERNEL::Position> postGlobleCanPos;
    std::map<double,KERNEL::Position> localCanPosMap;
    std::map<double,KERNEL::Position> globleCanPosMap;
    std::map<double,KERNEL::Position> canPosMap;
    
    int searchLevel1 = 2 * scout.getSight();
    int searchLevel2 = 4 * scout.getSight();
    int searchLevel3 = 8 * scout.getSight();
    
    //------------debugging---------------
    level1_radius = searchLevel1;
    level2_radius = searchLevel2;
    level3_radius = searchLevel3;
    circleCenter = scout.getPosition();
    //------------------------------------
    
    level1 = level2 = level3 = false;
    
    int iteraterLevel1 = (int)pow((double)searchLevel1, 2.0) * 10;
    int iteraterLevel2 = (int)pow((double)searchLevel2, 2.0) * 10;
    int iteraterLevel3 = (int)pow((double)searchLevel3, 2.0) * 10;
    int iteraterGlobal = dynamicMap->getHeight() * dynamicMap->getWidth() * 10;
    int iteraterLevel;
    
    std::vector<KERNEL::Position> localCanPosLevel1;
    std::vector<KERNEL::Position> localCanPosLevel2;
    std::vector<KERNEL::Position> localCanPosLevel3;
    std::vector<KERNEL::Position> localCanPosVec;
    
    std::vector<pos2>    res;
    int                  travelDistance;
    
    alpha = 0.4;
    beta = 0.6;
    
    double               tileExploredUtility = 0.0;
    double               segExploredUtility = 0.0;
    double               featureExploredUtility = 0.0;
    double               costComponent = 0.0;
    double               utilityComponent = 0.0;
    double               sumEvaluation = 0.0;
    
    bool                 repeatFlag = false;
    
    //----------------debugging-------------------
    std::vector<KERNEL::Position> polygonPoints;
    //--------------------------------------------
    
    std::set<esPolygon*> exploredPolygons = esMapAnalysis::getInstance()->getExploredPolygons();
    
#if 0
    //-------------------debugging-------------------------------------------------
    printf("Transferred explored polygon size %lu \n", exploredPolygons.size());
    
    std::set<esPolygon*>::iterator pit = exploredPolygons.begin();
    std::set<esPolygon*>::iterator pend = exploredPolygons.end();
    
    for ( unsigned int i = 0; pit != pend; ++ pit, ++ i) {
        printf("Transferred polygon %u size %lu \n", i, (*pit)->pointSet.size());
        if ( (*pit)->getHoles().size() != 0) {
            printf("Transferred polygon %u holes size %lu \n", i, (*pit)->getHoles().size());
            for ( unsigned int j = 0; j < (*pit)->getHoles().size(); ++ j) {
                printf("Transferred polygon %u hole %u size %lu \n", i, j, (*pit)->getHoles()[j].pointSet.size());
            }
        }
    }
    //-------------------------------------------------------------------------------
#endif
    
    //-------------------------debugging------------------------------
    std::set<esPolygon*>::iterator it = exploredPolygons.begin();
    std::set<esPolygon*>::const_iterator end = exploredPolygons.end();
    
    for (; it != end; ++it) {
        for (unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
            globleCanPos.push_back((*it)->pointSet[i]);
        }
        if ((*it)->getHoles().size() != 0) {
            for (unsigned int j = 0; j < (*it)->getHoles().size(); ++j) {
                for (unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); k ++ ) {
                    globleCanPos.push_back((*it)->getHoles()[j].pointSet[k]);
                }
            }
        }
    }
    
    it = exploredPolygons.begin();
    for (; it != end; ++it) {
        for (unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
            polygonPoints.push_back((*it)->pointSet[i]);
        }
        if ((*it)->getHoles().size() != 0) {
            for (unsigned int j = 0; j < (*it)->getHoles().size(); ++j) {
                for (unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); k ++) {
                    polygonPoints.push_back((*it)->getHoles()[j].pointSet[k]);
                }
            }
        }
    }
    
    //-----------------------------------------------------------------------------------
    
    if (globleCanPos.size() == 0) {
        return;
    }
    
    //-------------debugging-------------------------------------------------
    debugmesh.clear();
    
    debugmesh.setMode(OF_PRIMITIVE_POINTS);
    
    for ( unsigned int i = 0; i < polygonPoints.size(); ++ i) {
        debugmesh.addVertex(ofVec2f(polygonPoints[i].x, polygonPoints[i].y));
    }
    //-----------------------------------------------------------------------
    
    for ( unsigned int i = 0; i < globleCanPos.size(); ++ i) {
        
        for (unsigned int j = 0; j < visitNodes.size(); ++j) {
            if ( globleCanPos[i].x == visitNodes[j].pos.x
                && globleCanPos[i].y == visitNodes[j].pos.y) {
                repeatFlag = true;
                break;
            }
        }
        
        if ( repeatFlag) {
            repeatFlag = false;
            continue;
        }
        
        if ( globleCanPos[i].x < 0 || globleCanPos[i].x > dynamicMap->getWidth()
            || globleCanPos[i].y < 0 || globleCanPos[i].y > dynamicMap->getHeight()) {
            continue;
        }
        
        postGlobleCanPos.push_back(globleCanPos[i]);
        
        float distanceToCenter = sqrt( pow((double)(globleCanPos[i].x - scout.x()), 2.0) + pow((double)(globleCanPos[i].y - scout.y()), 2.0));
        if ( distanceToCenter < searchLevel3 ) {
            localCanPosLevel3.push_back(globleCanPos[i]);
            if ( distanceToCenter < searchLevel2 ) {
                localCanPosLevel2.push_back(globleCanPos[i]);
                if ( distanceToCenter < searchLevel1) {
                    localCanPosLevel1.push_back(globleCanPos[i]);
                }
            }
        }
    }
    
    if ( localCanPosLevel1.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel1, exploredPolygons, target);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level1 = true;
            return;
        }
    }
    
    if ( localCanPosLevel2.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel2, exploredPolygons, target);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level2 = true;
            return;
        }
    }
    
    if ( localCanPosLevel3.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel3, exploredPolygons, target);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level3 = true;
            return;
        }
    }
    
    if ( postGlobleCanPos.size() != 0) {
        utilityDecision(canPosMap, postGlobleCanPos, exploredPolygons, target);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            return;
        }
    }
    
#if 0
    printf("visited nodes ----------\n");
    for ( unsigned int i = 0; i < visitNodes.size(); ++ i) {
        printf( "Node %u, (%d, %d)", i, visitNodes[i].pos.x, visitNodes[i].pos.y );
    }
#endif
    
    //--------debug----------
    debugMesh2.clear();
    debugMesh2.setMode(OF_PRIMITIVE_POINTS);
    debugMesh2.addVertex(target);
    //-----------------------------------
    
}

void esStrFBMC::utilityDecision(std::map<double, KERNEL::Position> &resoultMap, std::vector<KERNEL::Position> canPosVec, std::set<esPolygon *> exploredPolygons, ofVec2f& target){
    
    resoultMap.clear();
    
    int                  travelDistance = 0.0;
    double               tileExploredUtility = 0.0;
    double               segExploredUtility = 0.0;
    double               featureExploredUtility = 0.0;
    double               costComponent = 0.0;
    double               utilityComponent = 0.0;
    double               sumEvaluation = 0.0;
    
    KERNEL::Position scoutLocation = KERNEL::Position( scoutBot.x(), scoutBot.y());
    
    bool repeatFlag = false;
    
    for ( unsigned int i = 0; i < canPosVec.size(); ++i) {
        
        ofVec2f candidatePos = ofVec2f((float)canPosVec[i].x, (float)canPosVec[i].y);
        travelDistance = scoutLocation.getDistance(canPosVec[i]);
        costComponent = exp( scoutBot.getRadius() - travelDistance);
        
        tileExploredUtility = calculateTilePercentage(exploredMap, scoutBot, candidatePos);
        segExploredUtility = calculateSegPercentage(exploredMap, scoutBot, candidatePos);
        featureExploredUtility = calculateFeaPercentage(exploredMap, scoutBot, candidatePos);
        
        utilityComponent = 0.4 * tileExploredUtility + 0.4 * segExploredUtility + 0.2 * featureExploredUtility;
        //utilityComponent = tileExploredUtility;
        
        sumEvaluation = alpha * costComponent + beta * utilityComponent;
        sumEvaluation = -sumEvaluation;
        
        resoultMap.insert(std::pair<double, KERNEL::Position>(sumEvaluation, canPosVec[i]));
        
    }
    
    if ( resoultMap.size() != 0){
        
        target = ofVec2f((float)resoultMap.begin()->second.x, (float)resoultMap.begin()->second.y);
        esNode nextTarget;
        nextTarget.pos = KERNEL::Position(resoultMap.begin()->second);
        nextTarget.visitFlag = true;
        visitNodes.push_back(nextTarget);
    }
    
}

void esStrFBMC::debugDraw(){
    
    ofPushStyle();
    ofFill();
    ofSetColor(255, 255, 0);
    glPointSize(4.0);
    debugmesh.draw();
    ofSetColor(0, 255, 0);
    debugMesh2.draw();
    
    ofSetColor(0, 255, 255);
    ofNoFill();
    if (level1) {
        ofCircle(circleCenter.x, circleCenter.y, level1_radius);
    }
    if (level2) {
        ofCircle(circleCenter.x, circleCenter.y, level2_radius);
    }
    if (level3) {
        ofCircle(circleCenter.x, circleCenter.y, level3_radius);
    }
    ofPopStyle();
    
}