//
//  esExtractPolygons.cpp
//  ExplorationStrategy
//
//  Created by Chen Si on 14/07/2014.
//
//

#include "esExtractPolygons.h"
#include "functions.h"

void rotateCw(int &x, int &y);
void rotateCcw(int &x, int &y);
void simplify(esPolygon &Polygon, double error_tol);

bool orderPolygonsDecreasingArea(const esPolygon &a, const esPolygon &b)
{
    return a.getArea() > b.getArea();
}

void extractPolygons(const KERNEL::RectangleArray<bool> &walkability
                      ,const std::list<esConnectedComponent> &components
                      ,std::vector<esPolygon> &polygons)
{
    KERNEL::Position pos;
    std::vector<esPolygon> walkable_polygons;

    for(std::list<esConnectedComponent>::const_iterator c = components.begin(); c != components.end(); c++)
    {
        bool walkable = c->isWalkable();
        
        int nextx = 1;
        int nexty = 2;
        
        int cx = (int)c->topLeft().x;
        int cy = (int)c->topLeft().y;
        
        bool adjcol[3][3];
        
        esPolygon newpoly;
        newpoly.pointSet.push_back(KERNEL::Position(cx,cy));
        
        bool first=true;
        
        while( cx != newpoly.pointSet[0].x || cy != newpoly.pointSet[0].y || first)
        {
            first = false;
            
            for(int i = 0; i < 3; i++) {
                for(int j = 0;j < 3; j++) adjcol[i][j] = !walkable;
            }
            
            adjcol[1][1] = walkability[cx][cy];
            
            if (cx > 0 && cy > 0)
                adjcol[0][0] = walkability[cx-1][cy-1];
            
            if (cy > 0)
                adjcol[1][0] = walkability[cx][cy-1];
            
            if (cx < (int)walkability.getWidth()-1 && cy > 0)
                adjcol[2][0] = walkability[cx+1][cy-1];
            
            if (cx > 0)
                adjcol[0][1] = walkability[cx-1][cy];
            
            if (cx < (int)walkability.getWidth()-1)
                adjcol[2][1] = walkability[cx+1][cy];
            
            if (cx > 0 && cy<(int)walkability.getHeight()-1)
                adjcol[0][2] = walkability[cx-1][cy+1];
            
            if (cy < (int)walkability.getHeight() - 1)
                adjcol[1][2] = walkability[cx][cy+1];
            
            if (cx < (int)walkability.getWidth() - 1 && cy < (int)walkability.getHeight()-1)
                adjcol[2][2] = walkability[cx+1][cy+1];
            
            bool done = false;
            
            rotateCw(nextx, nexty);
            rotateCw(nextx, nexty);
            
            if (adjcol[nextx][nexty] != walkable)
            {
                for(int count = 0;count <= 8 && adjcol[nextx][nexty] != walkable; count++) {
                    rotateCcw(nextx,nexty);
                if (walkable)
                        rotateCcw(nextx,nexty);
                    if (count==8) done=true;
                }
            }
            if (done) break;
            cx = cx + nextx-1;
            cy = cy + nexty-1;
            newpoly.pointSet.push_back(KERNEL::Position(cx,cy));
        }
        
        if (newpoly.getArea()>16)
        {
            if (walkable)
                walkable_polygons.push_back(newpoly);
            else
                polygons.push_back(newpoly);
        }
    }
    
    sort( polygons.begin(), polygons.end(), orderPolygonsDecreasingArea);
    sort( walkable_polygons.begin(), walkable_polygons.end(), orderPolygonsDecreasingArea);
    
    for(size_t i = 0; i < walkable_polygons.size(); i++)
    {
        if (walkable_polygons[i].getArea() >= 20)
        {
            for( size_t j = 0;j < polygons.size();j++ )
            {
                if ( polygons[j].getArea() > walkable_polygons[i].getArea() && polygons[j].isInside(walkable_polygons[i].pointSet[0]))
                {
                    polygons[j].holes.push_back(walkable_polygons[i]);
                    break;
                }
            }
        }
    }
    for(size_t i = 0;i < polygons.size();i++)
    {
        simplify(polygons[i],1.0);
        
        for(std::vector<esPolygon>::iterator h = polygons[i].holes.begin(); h != polygons[i].holes.end();h++)
        {
            simplify(*h,1.0);
        }
    }
    //log("Simplified polygons.");
}

void rotateCw(int &x,int &y) {
    if (x==0 && y==0) {
        x = 1;
        return;
    }
    if (x == 0 && y == 1) {
        y = 0;
        return;
    }
    if (x == 0 && y == 2) {
        y = 1;
        return;
    }
    if (x == 1 && y == 2) {
        x = 0;
        return;
    }
    if (x == 2 && y == 2) {
        x = 1;
        return;
    }
    if (x == 2 && y == 1) {
        y = 2;
        return;
    }
    if (x == 2 && y == 0) {
        y = 1;
        return;
    }
    if (x == 1 && y == 0) {
        x = 2;
        return;
    }
}

void rotateCcw(int &x,int &y) {
    if (x == 0 && y == 0) {
        y = 1;
        return;
    }
    if (x == 0 && y == 1) {
        y = 2;
        return;
    }
    if (x == 0 && y == 2) {
        x = 1;
        return;
    }
    if (x == 1 && y == 2) {
        x = 2;
        return;
    }
    if (x == 2 && y == 2) {
        y = 1;
        return;
    }
    if (x == 2 && y == 1) {
        y = 0;
        return;
    }
    if (x == 2 && y == 0) {
        x = 1;
        return;
    }
    if (x == 1 && y == 0) {
        x = 0;
        return;
    }
}

void simplify(esPolygon &polygon, double error_tol)
{
    polygon.pointSet.push_back(polygon.pointSet[0]);
    for(size_t i = 0;i + 1 < polygon.pointSet.size();i++)
    {
        size_t j = i + 1;
        int last_good_point = j;
        bool within_tol=true;
        while (within_tol && j<polygon.pointSet.size())
        {
            j++;
            if (j == polygon.pointSet.size())
                break;
            for(size_t k = i + 1;k < j;k++)
            {
                double dx = polygon.pointSet[i].x - polygon.pointSet[j].x;
                double dy = polygon.pointSet[i].y - polygon.pointSet[j].y;
                double d = sqrt(dx * dx + dy * dy);
                double nx = dy / d;
                double ny = -dx / d;
                double distance = abs((polygon.pointSet[k].x - polygon.pointSet[i].x) * nx+(polygon.pointSet[k].y - polygon.pointSet[i].y)*ny);
                
                if (distance >= error_tol)
                {
                    within_tol = false;
                    break;
                }
            }
            if (within_tol)
            {
                last_good_point = j;
            }
        }
        if (i + 1 != last_good_point)
        {
            polygon.pointSet.erase(polygon.pointSet.begin() + i + 1,polygon.pointSet.begin()+last_good_point);
        }
    }
    if ( polygon.pointSet[0]==polygon.pointSet.back())
    {
        polygon.pointSet.erase(polygon.pointSet.begin()+polygon.pointSet.size()-1);
    }
}

void floodFillwithComponent(const KERNEL::RectangleArray<bool> &read_map
                               ,const KERNEL::Position start
                               ,esConnectedComponent* component
                               ,int fill_type
                               ,KERNEL::RectangleArray<esConnectedComponent*> &write_map)
{
    if (component == NULL)
        return;
    int fill_count = 0;
    std::list< KERNEL::Position > q;
    q.push_back(start);
    
    while (!q.empty())
    {
        KERNEL::Position p = q.front();
        if (p.x < component->topLeft().x || ( p.x == component->topLeft().x && p.y < component->topLeft().y ))
        {
            component->setTopLeft(ofVec2f(p.x, p.y));
        }
        int x = (int)p.x;
        int y = (int)p.y;
        
        q.pop_front();
        
        if ( write_map[x][y] == NULL )
        {
            write_map[x][y] = component;
            
            int min_x = max( x - 1, 0);
            int max_x = min( (int)(x + 1),(int)(read_map.getWidth() - 1));
            int min_y = max( y - 1, 0);
            int max_y = min( (int)(y + 1),(int)(read_map.getHeight()-1));
            
            for(int ix = min_x; ix <= max_x; ix++)
            {
                for(int iy = min_y; iy <= max_y; iy++)
                {
                    int fill = 0;
                    if (fill_type == 0) {//4-directional movement
                        if (x == ix || y == iy) {
                            if (read_map[ix][iy] == component->isWalkable() && write_map[ix][iy]==NULL)
                            {
                                fill = 1;
                            }
                        }
                    } else if (fill_type == 1) {//limited 8-directional movement
                        if (x == ix || y == iy || read_map[x][iy] == component->isWalkable()
                            || read_map[ix][y] == component->isWalkable()) {
                            if (read_map[ix][iy] == component->isWalkable() && write_map[ix][iy] == NULL)
                            {
                                fill = 1;
                            }
                        }
                        
                    } else if (fill_type == 2) {//full 8-directional movement
                        if (read_map[ix][iy] == component->isWalkable() && write_map[ix][iy]==NULL)
                        {
                            fill = 1;
                        }
                    }
                    if (fill) {
                        fill_count++;
                        q.push_back(KERNEL::Position(ix,iy));
                    }
                }
            }
        }
    }
}

void findConnectedComponents(const KERNEL::RectangleArray<bool> &simplified_map
                               ,KERNEL::RectangleArray<esConnectedComponent*> &get_component
                               ,std::list<esConnectedComponent> &components)
{
    
    int currentID = 1;
    components.clear();
    get_component.resize(simplified_map.getWidth(), simplified_map.getHeight());
    
    for (unsigned int x = 0; x < simplified_map.getWidth(); x++) {
        for (unsigned int y = 0; y < simplified_map.getHeight(); y++) {
            get_component[x][y] = NULL;
        }
    }
    
    for (unsigned int x = 0; x < simplified_map.getWidth(); x++) {
        for (unsigned int y = 0; y < simplified_map.getHeight(); y++) {
            if (get_component[x][y] == NULL) {
                
                components.push_back(esConnectedComponent( currentID++, simplified_map[x][y]));
                esConnectedComponent *current_component = &(components.back());
                int fill_type = 0;
                if ( simplified_map[x][y]==false ) fill_type = 2;
                current_component->setTopLeft(ofVec2f(x, y));
                floodFillwithComponent(simplified_map, KERNEL::Position(x, y), current_component, fill_type, get_component);
                
            }
        }
    }
}


