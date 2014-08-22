#pragma once

#include "ofMain.h"
#include <set>

class Tile;
typedef ofPtr<Tile> TilePtr;

class ofxPathfinder {
public:	
	ofxPathfinder();
	void setup(ofPixels& pix);
	float find(int sourceX, int sourceY, int targetX, int targetY);
	ofPolyline path;
	
private:
	vector<TilePtr> tiles;
	set<TilePtr> openSet, closedSet;
	int w, h;
	TilePtr source, target;
	bool found;
	
	void setSource(int x, int y);
	void setTarget(int x, int y);
	TilePtr& getTile(int x, int y);
	bool safe(int x, int y) const;
	void finish();
	void consider(const TilePtr& parent, int x, int y);
};
