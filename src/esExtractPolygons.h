//
//  esExtractPolygons.h
//  ExplorationStrategy
//
//  Created by Chen Si on 12/07/2014.
//
//

#ifndef ExplorationStrategy_esExtractPolygons_h
#define ExplorationStrategy_esExtractPolygons_h


#include "esConnectedComponent.h"
#include "functions.h"
#include "RectangleArray.h"
#include <esPolygon.h>

void findConnectedComponents(const KERNEL::RectangleArray<bool> &simplified_map
                               ,KERNEL::RectangleArray<esConnectedComponent*> &get_component
                               ,std::list<esConnectedComponent> &components);

void extractPolygons(const KERNEL::RectangleArray<bool> &walkability
                      ,const std::list<esConnectedComponent> &components
                      ,std::vector<esPolygon> &polygons);


void floodFillwithComponent(const KERNEL::RectangleArray<bool> &read_map
                               ,const KERNEL::Position start
                               ,esConnectedComponent* component
                               ,int fill_type
                               ,KERNEL::RectangleArray<esConnectedComponent*> &write_map);



#endif
