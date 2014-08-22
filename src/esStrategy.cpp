//
//  esStrategy.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 20/07/2014.
//
//

#include "esStrategy.h"
#include "esMapAnalysis.h"


#define EPS 0.00001

bool esStrategy::isIncircle(KERNEL::Position center, int radius, KERNEL::Position node){
    return ( node.getDistance(center) <= radius );
}

float esStrategy::calculateTilePercentage(ofxTileMap *dynamicMap, Robot& scout, ofVec2f &target){
    
    int sumUnexplored = 0;
    int sum = 0;
    for( int i = - scout.getSight(); i <= scout.getSight(); i++){
        for( int j = - scout.getSight(); j <= scout.getSight(); j++){
            if (sqrt( pow((double)i, 2.0) + pow((double)j, 2.0)) <= scout.getSight()
                && ( i + (int)target.x ) > 0 && (i + (int)target.x) < dynamicMap->getWidth()
                && ( j + (int)target.y ) > 0 && (j + (int)target.y) < dynamicMap->getHeight()){
                sum ++;
                if( dynamicMap->getTileSafe(i + (int)target.x, j + (int)target.y) == UN_KNOWN){
                    sumUnexplored ++;
                }
            }
        }
    }
    
    if ( sum == 0) {
        return 0;
    }else{
        double sumD = (double)sum;
        return sumUnexplored / sumD;
    }
}

float esStrategy::calculateSegPercentage(ofxTileMap *dynamicMap, Robot &scout, ofVec2f &target){
    
    double sumUnexplored = 0.0;
    double sumExplored = 0.0;
    
    std::vector<std::vector<edgeNode> > proVertexVec;
    
    std::set<esPolygon*> exploredPolygon = esMapAnalysis::getInstance()->getExploredPolygons();
    
    std::set<esPolygon*>::iterator it = exploredPolygon.begin();
    std::set<esPolygon*>::iterator end = exploredPolygon.end();
    
    for (; it != end; ++ it) {
        
        std::vector<edgeNode>  vertexVec;
        
        for ( unsigned int i = 0; i < (*it)->pointSet.size(); i ++) {
            edgeNode node;
            node.pos = (*it)->pointSet[i];
            node.explorationFlag = evaluateEdgePoint(dynamicMap, node.pos);
            vertexVec.push_back(node);
        }
        
        proVertexVec.push_back(vertexVec);
        
        for ( unsigned int j = 0; j < (*it)->getHoles().size(); ++ j) {
            
            std::vector<edgeNode> holeVertexVec;
            
            for ( unsigned int k = 0; k < (*it)->getHoles()[j].pointSet.size(); ++ k) {
                
                edgeNode hNode;
                hNode.pos = (*it)->getHoles()[j].pointSet[k];
                hNode.explorationFlag = evaluateEdgePoint(dynamicMap, hNode.pos);
                holeVertexVec.push_back(hNode);
            }
            
            proVertexVec.push_back(holeVertexVec);
        }
    }
#if 0
    printf("There are %lu ploygons. \n", proVertexVec.size());
    //--------------------debugging-------------------------
    for (unsigned int i = 0; i < proVertexVec.size(); ++ i) {
        printf("There are %lu vertices in vector %u. \n", proVertexVec[i].size(), i);
        for ( unsigned int j = 0; j < proVertexVec[i].size(); ++ j) {
            printf("vectex (%u, %u) located in (%u, %u)", i, j, proVertexVec[i][j].pos.x, proVertexVec[i][j].pos.y);
            switch (proVertexVec[i][j].explorationFlag) {
                case WALKABLE:
                    printf("is walkable");
                    break;
                case UN_WALKABLE:
                    printf("is unwalkable");
                    break;
                case UN_KNOWN:
                    printf("is unknown");
                    break;
                case RESOURCE:
                    printf("is resource");
                    break;
                default:
                    printf("is error");
                    break;
            }
            printf("\n");
        }
    }
    //------------------------------------------------------
#endif
    

    for( unsigned int i = 0; i < proVertexVec.size(); ++ i){
        
        unsigned int insertAcount = 0;
        edgeNode iter;
        edgeNode trail;
        
        
        for ( unsigned int j = 0; j < proVertexVec[i].size(); ++ j) {
            
            KERNEL::Position ptPoint1;
            KERNEL::Position ptPoint2;
            
            iter = proVertexVec[i][j];
            
            if ( j < proVertexVec[i].size() - 1) {
                trail = proVertexVec[i][j+1];
            }else{
               // printf("reach the end of List\n");
                trail = proVertexVec[i][0];
            }
            //------------------------------------
            
            if ( lineInterCircle( iter.pos, trail.pos, KERNEL::Position((int)target.x, (int)target.y), scout.getSight(), ptPoint1, ptPoint2)) {
                //---------------------------------------
                //insertAcount ++;
                //printf("-----%u------iteration---%u-----", insertAcount,j);
                //printf("begin point (%u, %u), end point (%u, %u)\n", iter.pos.x, iter.pos.y, trail.pos.x, trail.pos.y);
                //--------------------------------------
                
                if ( ptPoint1.x > 0 && ptPoint1.x < dynamicMap->getWidth()
                    && ptPoint1.y > 0 && ptPoint1.y < dynamicMap->getHeight()) {
                    
                    edgeNode hp1Node;
                    hp1Node.pos = ptPoint1;
                    hp1Node.explorationFlag = evaluateEdgePoint(dynamicMap, hp1Node.pos);
                    
                    try {
                        proVertexVec[i].insert( proVertexVec[i].begin() + j, hp1Node);
                    } catch (...) {
                        proVertexVec[i].insert( proVertexVec[i].begin() + j, hp1Node);
                    }
                    
                    ++j;
                    //----------------------------------------
                    //printf("First insert position (%u, %u) \n", ptPoint1.x, ptPoint1.y);
                    //----------------------------------------
                }
                
                if ( ptPoint2.x > 0 && ptPoint2.x < dynamicMap->getWidth()
                    && ptPoint2.y > 0 && ptPoint2.y < dynamicMap->getHeight()) {
                    
                    edgeNode hp2Node;
                    hp2Node.pos = ptPoint2;
                    hp2Node.explorationFlag = evaluateEdgePoint(dynamicMap, hp2Node.pos);
                    
                    try {
                        proVertexVec[i].insert( proVertexVec[i].begin() + j, hp2Node);
                    } catch (...) {
                        proVertexVec[i].insert( proVertexVec[i].begin() + j, hp2Node);
                    }
                    
                    ++ j;
                    //------------------------------------------
                    //printf("Second insert position (%u, %u)\n", ptPoint2.x, ptPoint2.y);
                    //------------------------------------------
                }
            }
            
        }
        
    }
    
    for( unsigned int i = 0; i < proVertexVec.size(); ++ i){
        
        edgeNode iter;
        edgeNode trail;
        
        for ( unsigned int j = 0; j < proVertexVec[i].size(); ++j) {
            
            iter = proVertexVec[i][j];
            if ( j < proVertexVec[i].size() ) {
                trail = proVertexVec[i][j+1];
            }else{
                trail = proVertexVec[i][0];
            }
            
            float distanceP1 = sqrt(pow((double)(iter.pos.x - scout.getPosition().x), 2.0)
                                    + pow((double)(iter.pos.y - scout.getPosition().y), 2.0));
            
            float distanceP2 = sqrt(pow((double)(trail.pos.x - scout.getPosition().x), 2.0)
                                    + pow((double)(trail.pos.y - scout.getPosition().y), 2.0));
            
            if ( ( distanceP1 <= scout.getSight() + 2 ) && ( distanceP2 <= scout.getSight() + 2)) {
                double distance_ = iter.pos.getDistance(trail.pos);
                if ( iter.explorationFlag == UN_WALKABLE
                    && trail.explorationFlag == UN_WALKABLE) {
                    sumExplored += distance_;
                }else{
                    sumUnexplored += distance_;
                }
                
            }
            
        }
        
    }

    if ( sumUnexplored + sumExplored < EPS) {
        return 0.0;
    }else{
        return sumUnexplored / ( sumUnexplored + sumExplored );
    }
    
}

void esStrategy::recalSegVertices(ofxTileMap *dynamicMap, Robot &scout, ofVec2f &target){
    
}

unsigned char esStrategy::evaluateEdgePoint(ofxTileMap *dynamicMap, KERNEL::Position &vertex){
    
    int sumUnknown = 0;
    int sumWalkable = 0;
    int sumObstacle = 0;
    
    int width = dynamicMap->getWidth();
    int height = dynamicMap->getHeight();
    
    for (int i = vertex.x - 1; i <= vertex.x + 1; ++ i) {
        for ( int j = vertex.y - 1; j <= vertex.y + 1; ++j) {
            
            if ( i < 0 || i > width
                || j < 0 || j > height) {
                sumObstacle ++;
                continue;
            }
            
            switch (dynamicMap->getTileSafe(i, j)) {
                case UN_KNOWN:
                    sumUnknown ++;
                    break;
                case UN_WALKABLE:
                    sumObstacle ++;
                    break;
                case WALKABLE:
                    sumWalkable ++;
                    break;
                default:
                    sumObstacle ++;
                    break;
            }
        }
    }
    
    if ( sumObstacle == sumUnknown &&
        sumObstacle == 0) {
        return WALKABLE;
    }else if( sumObstacle != 0){
        return UN_WALKABLE;
    }else{
        return UN_KNOWN;
    }
}

bool esStrategy::lineInterCircle(const KERNEL::Position ptStart, const KERNEL::Position ptEnd, const KERNEL::Position ptCenter,
                                 const float Radius, KERNEL::Position &ptInter1, KERNEL::Position &ptInter2){
    
    ptInter1.x = ptInter1.y = 65535;
    ptInter2.x = ptInter2.y = 65535;
    
    if ( ptStart.getDistance(ptCenter) < Radius
        && ptEnd.getDistance(ptCenter) < Radius) {
        return false;
    }
    
    float fDis = sqrt(pow((double)(ptEnd.x - ptStart.x), 2.0) + pow((double)(ptEnd.y - ptStart.y), 2.0));
    
    float d_x = ( ptEnd.x - ptStart.x) / fDis;
    float d_y = ( ptEnd.y - ptStart.y) / fDis;
    
    float e_x = (float)(ptCenter.x - ptStart.x);
    float e_y = (float)(ptCenter.y - ptStart.y);
    
    float a = e_x * d_x + e_y * d_y;
    float a2 = a * a;
    
    float e2 = e_x * e_x + e_y * e_y;
    
    float r2 = Radius * Radius;
    
    if ((r2 - e2 + a2) < 0) {
        
        return false;
        
    }else{
        
        float f =sqrt(r2 - e2 + a2);
        
        float t = a - f;
        
        if ( ((t - 0.0) > -EPS) && (t - fDis) < EPS) {
            ptInter1.x = ptStart.x + t * d_x;
            ptInter1.y = ptStart.y + t * d_y;
        }
        
        t = a + f;
        
        if ( ((t - 0.0) > -EPS) && ( t - fDis) < EPS) {
            ptInter2.x = ptStart.x + t * d_x;
            ptInter2.y = ptStart.y + t * d_y;
        }
        
        return true;
    }
}

float esStrategy::calculateFeaPercentage(ofxTileMap *dynamicMap, Robot &scout, ofVec2f &target){
    //calculate
    int sumUnexplored = 0;
    int sum = 0;
    int sumResource = 0;
    
    for( int i = - scout.getSight(); i <= scout.getSight(); i++){
        for( int j = - scout.getSight(); j <= scout.getSight(); j++){
            if (sqrt( pow((double)i, 2.0) + pow((double)j, 2.0)) <= scout.getSight()
                && ( i + (int)target.x ) > 0 && (i + (int)target.x) < dynamicMap->getWidth()
                && ( j + (int)target.y ) > 0 && (j + (int)target.y) < dynamicMap->getHeight()){
                sum ++;
                if( dynamicMap->getTileSafe(i + (int)target.x, j + (int)target.y) == UN_KNOWN){
                    sumUnexplored ++;
                }else if( dynamicMap->getTileSafe(i + (int)target.x, j + (int)target.y) == RESOURCE){
                    sumResource ++;
                }
            }
        }
    }
    
    if ( sum == 0 ) {
        return 0;
    }else{
        double sumD = (double)sum;
        double sumExplored = (double)(sum - sumUnexplored);
        return ( sumResource * sumUnexplored) / ( sumD * sumExplored );
    }
    
#if 0
    if ( sum == 0 || sum == sumUnexplored) {
        return 0;
    }else{
        double sumD = (double)sum;
        double sumExplored = (double)(sum - sumUnexplored);
        double exploredPercent = sumResource / sumExplored;
        if ( exploredPercent < 0.1) {
            return 4 * ( sumResource * sumUnexplored) / ( sumD * sumExplored );
        }else if( exploredPercent >= 0.1 && exploredPercent <= 0.5){
            return 2 * ( sumResource * sumUnexplored) / ( sumD * sumExplored );
        }else{
            return ( sumResource * sumUnexplored) / ( sumD * sumExplored );
        }
        
    }
#endif
}

bool esStrategy::isReachable(std::set<esPolygon *> polygons, KERNEL::Position p1, KERNEL::Position p2){
    
    std::set<esPolygon*>::iterator it = polygons.begin();
    std::set<esPolygon*>::const_iterator end = polygons.end();
    
    for ( ; it != end; ++it) {
        if ( (*it)->isInside(p1)
            && (*it)->isInside(p2)) {
            return true;
        }
    }
    
    return false;
}

esStrategy::~esStrategy(){
    printf("call esStrategy destructor!\n");
    visitNodes.clear();
    proVertexSet.clear();
}

void esStrategy::setAlgorithmConfig(algorithmConfig confi){
    config = confi;
}

KERNEL::Position esStrategy::searchWalkableNeighbor(ofxTileMap *dynamicMap, KERNEL::Position original){
    if ( dynamicMap->getTileSafe(original.x, original.y) == WALKABLE) {
        return original;
    }else{
        for(int i = original.x - 2; i <= original.x + 2; i ++){
            for (int j = original.y - 2; j <= original.y + 2; j ++) {
                if ( i < 0 || i > dynamicMap->getWidth() || j < 0 || j > dynamicMap->getHeight()) {
                    continue;
                }else{
                    if (dynamicMap->getTileSafe(i, j) == WALKABLE) {
                        return KERNEL::Position(i, j);
                    }
                }
            }
        }
        return original;
    }
}
