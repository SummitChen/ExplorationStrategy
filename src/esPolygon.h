//
//  esPolygon.h
//  ExplorationStrategy
//
//  Created by Chen Si on 5/07/2014.
//
//

#ifndef ExplorationStrategy_esPolygon_h
#define ExplorationStrategy_esPolygon_h

#include <vector>
#include "Position.h"


class esPolygon{
public:
    esPolygon();
    esPolygon(esPolygon const & b);
    
    double           getArea() const;
    double           getPerimeter() const;
    
    KERNEL::Position          getCenter() const;
    KERNEL::Position          getNearestPoint(KERNEL::Position p) const;
    
    std::vector<esPolygon> getHoles() const;
    
    bool             isInside(KERNEL::Position p) const;
    
    std::vector<KERNEL::Position> pointSet;
    
    std::vector<esPolygon> holes;
};

#endif
