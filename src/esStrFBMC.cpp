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
    std::vector<KERNEL::Position> postGlobleCanPosArray1;
    std::vector<KERNEL::Position> postGlobleCanPosArray2;
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
    std::vector<KERNEL::Position> localCanPosLevel3_2;
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
                for (unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); k += 2) {
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
        
        if ( i % 2 == 0) {
            postGlobleCanPosArray2.push_back(globleCanPos[i]);
        }else{
            postGlobleCanPosArray1.push_back(globleCanPos[i]);
        }
        
    }
    
    for ( unsigned int i = 0; i < postGlobleCanPosArray1.size(); ++ i) {
        
        float distanceToCenter = sqrt( pow((double)(postGlobleCanPosArray1[i].x - scout.x()), 2.0) + pow((double)(postGlobleCanPosArray1[i].y - scout.y()), 2.0));
        if ( distanceToCenter < searchLevel3 ) {
            localCanPosLevel3.push_back(postGlobleCanPosArray1[i]);
            if ( distanceToCenter < searchLevel2 ) {
                localCanPosLevel2.push_back(postGlobleCanPosArray1[i]);
                if ( distanceToCenter < searchLevel1) {
                    localCanPosLevel1.push_back(postGlobleCanPosArray1[i]);
                }
            }
        }
    }
    
    for ( unsigned int i = 0; i < postGlobleCanPosArray2.size(); ++ i) {
        
        float distanceToCenter = sqrt( pow((double)(postGlobleCanPosArray2[i].x - scout.x()), 2.0) + pow((double)(postGlobleCanPosArray2[i].y - scout.y()), 2.0));
        if ( distanceToCenter < searchLevel3 ) {
            localCanPosLevel3_2.push_back(postGlobleCanPosArray2[i]);
        }
    }
    
    if ( localCanPosLevel1.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel1, exploredPolygons, target, true);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level1 = true;
            goto display;
        }
    }
    
    if ( localCanPosLevel2.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel2, exploredPolygons, target, true);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level2 = true;
            goto display;
        }
    }
    
    if ( localCanPosLevel3.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel3, exploredPolygons, target, true);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level3 = true;
            goto display;
        }
    }
    
    if ( localCanPosLevel3_2.size() != 0) {
        utilityDecision(canPosMap, localCanPosLevel3_2, exploredPolygons, target, true);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            level3 = true;
            goto display;
        }
    }

    
    if ( postGlobleCanPosArray1.size() != 0) {
        utilityDecision(canPosMap, postGlobleCanPosArray1, exploredPolygons, target, false);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            goto display;
        }
    }
    
    if ( postGlobleCanPosArray2.size() != 0) {
        utilityDecision(canPosMap, postGlobleCanPosArray2, exploredPolygons, target, false);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
            goto display;
        }
    }
    
display:
    
    //float tilePercentage = calculateTilePercentage(exploredMap, scoutBot, target);
    //float segPercentage  = calculateSegPercentage(exploredMap, scoutBot, target);
    //float feaPercentage  = calculateFeaPercentage(exploredMap, scoutBot, target);
    
    //printf("target - (%d, %d), tile - %f, seg - %f, feature - %f \n", (int)target.x, (int)target.y, tilePercentage, segPercentage, feaPercentage);
    
    
    //--------debug----------
    debugMesh2.clear();
    debugMesh2.setMode(OF_PRIMITIVE_POINTS);
    debugMesh2.addVertex(target);
    //-----------------------------------
    
}

void esStrFBMC::utilityDecision(std::map<double, KERNEL::Position> &resoultMap, std::vector<KERNEL::Position> canPosVec, std::set<esPolygon *> exploredPolygons, ofVec2f& target, bool localSearch){
    
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
        
       // utilityComponent = 0.4 * tileExploredUtility + 0.4 * segExploredUtility + 0.2 * featureExploredUtility;
        
        if ( fabs((config.getGrid() + config.getSegment() + config.getFeature()) - 1.0) < 0.002) {
        
            utilityComponent = config.getGrid() * tileExploredUtility + config.getSegment() * segExploredUtility + config.getFeature() * featureExploredUtility;
        
        }else{
        
            utilityComponent = 0.4 * tileExploredUtility + 0.4 * segExploredUtility + 0.2 * featureExploredUtility;
        
        }

        
        if ( localSearch && utilityComponent < 0.2) {
            continue;
        }
        
        if ( fabs((config.getAlpha() + config.getBeta()) - 1.0) < 0.002 ) {
            sumEvaluation = config.getAlpha() * costComponent + config.getBeta() * utilityComponent;
        }else{
            sumEvaluation = 0.4 * costComponent + 0.6 * utilityComponent;
        }
        
        sumEvaluation = -sumEvaluation;
        
        resoultMap.insert(std::pair<double, KERNEL::Position>(sumEvaluation, canPosVec[i]));
        
    }
    
    if ( resoultMap.size() != 0){
        
        //target = ofVec2f((float)resoultMap.begin()->second.x, (float)resoultMap.begin()->second.y);
        KERNEL::Position walkableTarget = searchWalkableNeighbor(exploredMap, resoultMap.begin()->second);
        target = ofVec2f(walkableTarget.x, walkableTarget.y);
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