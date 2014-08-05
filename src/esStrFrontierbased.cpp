//
//  esStrFrontierbased.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 20/07/2014.
//
//

#include "esStrFrontierbased.h"
#include "esMapAnalysis.h"

void esStrFrontierbased::calCandidatePosition(ofxTileMap *solidMap, ofxTileMap *dynamicMap, bool &resetTarget, ofVec2f &target, Robot &scout, ofxMapRouting *routing){
    
    esMapAnalysis::getInstance()->analyzeExploredArea(dynamicMap);
    
    exploredMap = dynamicMap;
    scoutBot = scout;
    
    KERNEL::Position scoutLocation = KERNEL::Position(scout.x(), scout.y());
    
    
    std::vector<KERNEL::Position> globleCanPos;
    std::vector<KERNEL::Position> postGlobleCanPos;
    std::map<double,KERNEL::Position> canPosMap;

    
    std::vector<pos2>    res;
    int                  travelDistance;
    
    alpha = 0.4;
    beta = 0.6;
    
    double               tileExploredUtility = 0.0;
    double               segExploredUtility = 0.0;
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
    

    std::set<esPolygon*>::iterator it = exploredPolygons.begin();
    std::set<esPolygon*>::const_iterator end = exploredPolygons.end();
    
    for (; it != end; ++it) {
        for (unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
            globleCanPos.push_back((*it)->pointSet[i]);
        }
        if ((*it)->getHoles().size() != 0) {
            for (unsigned int j = 0; j < (*it)->getHoles().size(); ++j) {
                for (unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); k ++) {
                    globleCanPos.push_back((*it)->getHoles()[j].pointSet[k]);
                }
            }
        }
    }

    //------------------debugging-----------------------------------------
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
    //-------------------------------------------------------------------
    
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
        
        if ( repeatFlag ) {
            repeatFlag = false;
            continue;
        }
        
        if ( globleCanPos[i].x < 0 || globleCanPos[i].x > dynamicMap->getWidth()
            || globleCanPos[i].y < 0 || globleCanPos[i].y > dynamicMap->getHeight()) {
            continue;
        }
        
        postGlobleCanPos.push_back(globleCanPos[i]);
        
    }
    
    if ( globleCanPos.size() != 0) {
        utilityDecision(canPosMap, postGlobleCanPos, exploredPolygons, target);
        if ( canPosMap.size() != 0) {
            resetTarget = true;
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

void esStrFrontierbased::utilityDecision(std::map<double, KERNEL::Position> &resoultMap, std::vector<KERNEL::Position> canPosVec, std::set<esPolygon *> exploredPolygon, ofVec2f &target){

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
        
        utilityComponent = 0.5 * tileExploredUtility + 0.5 * segExploredUtility;
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
void esStrFrontierbased::debugDraw(){
    
    ofPushStyle();
    ofSetColor(255, 255, 0);
    glPointSize(4.0);
    debugmesh.draw();
    ofSetColor(0, 255, 0);
    debugMesh2.draw();
    ofPopStyle();
    
}