/*
 *  ofxTileMap.h
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 25/02/12.
 *  Copyright 2012 uri.cat. All rights reserved.
 *
 */

#pragma once

//some tile values
#define UN_WALKABLE 0
#define UN_KNOWN 100
#define RESOURCE 200
#define WALKABLE 255


#include "ofMain.h"
#include "Position.h"
#include <set>

class mapFeature{

public:
    KERNEL::Position pos;
    float            radius;
    string           name;
    
   // mapFeature& operator=(const mapFeature&);
};

#if 0
mapFeature& mapFeature::operator=(const mapFeature &other){
    this->pos = other.pos;
    this->radius = other.radius;
    this->name = other.name;
}
#endif

class ofxTileMap{
	
public:

	ofxTileMap();
	ofxTileMap( int width, int height );
	ofxTileMap( string imagePath );
	~ofxTileMap();
	
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	
	void draw();

	void randomize(int blockW , int blockH, int streetW); // in case u dont use an image
	
	unsigned char getTile(int x, int y);
	unsigned char getTileSafe(int x, int y);
	unsigned char * getTileAddressSafe(int x, int y);
	
	void setTile(int x, int y, unsigned char val);
	void setTileSafe(int x, int y, unsigned char val);
	
	void updateMapImage();
    void loadFeatureData(string dataPath);

	int width;
	int height;
	unsigned char * map;
	ofImage mapImg;
    
    std::vector<mapFeature> mapFeatures;
    int accFreeTile;
    int resourceTile;
};
