//
//  esPolygon.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 5/07/2014.
//
//

#include "esPolygon.h"
#include "functions.h"

esPolygon::esPolygon(){

}

esPolygon::esPolygon(esPolygon const & b){

    for ( unsigned int i = 0; i < b.pointSet.size(); i++) {
        this->pointSet.push_back(b.pointSet[i]);
    }
        
    if ( !b.holes.empty()) {
        this->holes = b.getHoles();
    }
}

double esPolygon::getArea() const
{
    if ( pointSet.size() < 3) return 0;
    
    double a = 0;
    for(unsigned int i = 0; i + 1 < pointSet.size();i++)
    {
        a+=(double)(this->pointSet[i].x)*(this->pointSet[i+1].y)-(double)(this->pointSet[i+1].x)*(this->pointSet[i].y);
    }
    a += this->pointSet.back().x * this->pointSet.front().y - this->pointSet.front().x * this->pointSet.back().y;
    a /= 2;
    a = fabs(a);
    return a;

}

double esPolygon::getPerimeter() const{
    
    if (this->pointSet.size()<2) return 0;
    
    double p = 0;
    
    for(unsigned int i = 0;i + 1 < this->pointSet.size();i++)
    {
        p += this->pointSet[i].getDistance(this->pointSet[i + 1]);
    }
    p += this->pointSet.back().getDistance(this->pointSet.front());
    return p;
}

KERNEL::Position esPolygon::getCenter() const{
    double a = getArea();
    double cx = 0;
    double cy = 0;
    double temp;
    for (unsigned int i = 0, j = 1; i < this->pointSet.size(); i++, j++ ) {
        if ( j == this->pointSet.size()) {
            j = 0;
            temp = (double)(this->pointSet[i].x) * (this->pointSet[j].y) - (double)(this->pointSet[j].x) * (this->pointSet[i].y);
            cx += (this->pointSet[i].x + this->pointSet[j].x) * temp;
            cy += (this->pointSet[i].y + this->pointSet[j].y) * temp;
        }
        
        cx = cx / (6.0 * a);
        cy = cy / (6.0 * a);
        
        return KERNEL::Position((int)cx, (int)cy);
    }
    
}

KERNEL::Position esPolygon::getNearestPoint(KERNEL::Position p) const
{
    double x3 = p.x;
    double y3 = p.y;
    KERNEL::Position minp = KERNEL::Positions::Unknown;
    
    int j = 1;
    double mind2 = -1;
    
    for(int i = 0;i < (int)this->pointSet.size();i++)
    {
        j= (i+1) % pointSet.size();
        
        double x1 = pointSet[i].x;
        double y1 = pointSet[i].y;
        double x2 = pointSet[j].x;
        double y2 = pointSet[j].y;
        
        double u = ((x3-x1)*(x2-x1)+(y3-y1)*(y2-y1))/((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        
        if (u<0) u = 0;
        if (u>1) u = 1;
        double x = x1 + u * (x2-x1);
        double y = y1 + u * (y2-y1);
        double d2=(x-x3) * (x-x3) + (y-y3) * (y-y3);
        if ( mind2 < 0 || d2 < mind2)
        {
            mind2 = d2;
            minp = KERNEL::Position((int)x,(int)y);
        }
    }
    for(std::vector<esPolygon>::const_iterator i = holes.begin();i!=holes.end();i++)
    {
        KERNEL::Position hnp = i->getNearestPoint(p);
        if ( hnp.getDistance(p) < minp.getDistance(p))
            minp=hnp;
    }
    return minp;
}
std::vector<esPolygon> esPolygon::getHoles() const
{
    return holes;
}

//expected to be implemented
bool esPolygon::isInside(KERNEL::Position p) const{
    bool flag = false;
    unsigned int nVert = pointSet.size();
    float *vertX = new float[nVert];
    float *vertY = new float[nVert];
    
    float p_x = (float)p.x;
    float p_y = (float)p.y;
    
    for (unsigned int i = 0; i < nVert; i ++) {
        vertX[i] = pointSet[i].x;
        vertY[i] = pointSet[i].y;
    }
    
#if 0
    for (int i = 0, j = nVert - 1; i < nVert; j = i++) {
        if ( ((vertY[i] > p_y) != (vertY[j] > p_y)) &&
            (p_x < (vertX[j]-vertX[i]) * (p_y - vertY[i]) / (vertY[j]-vertY[i]) + vertX[i]) )
            flag = !flag;
    }
#endif
    
    for (int i = 0, j = nVert - 1; i < nVert; j = i++) {
        if ( ((vertY[i] < p_y && vertY[j] >= p_y)
            || (vertY[j] < p_y && vertY[i] >= p_y))
            && (vertX[i] <= p_x || vertX[j] <= p_x)) {
            flag ^= (vertX[i]+( p_y - vertY[i])/(vertY[j]-vertY[i])*(vertX[j]-vertX[i]) < p_x);
        }
    }
    return flag;
}