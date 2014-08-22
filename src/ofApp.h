#pragma once

#include "ofMain.h"
#include "ofxMapRouting.h"
#include "ofxTileMap.h"
#include "ofxTimeMeasurements.h"
#include "ofxSimpleGuiToo.h"
#include "Robot.h"
#include "ofEsLog.h"
#include "esBotModel.h"
#include "ofEsSettings.h"

#define BOT           0
#define BOT_REPLAY    1
#define HUMAN         2
#define HUMAN_REPLAY  3

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    ofApp();
    ~ofApp();
    
    void keyPressed( int key );
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
#if 0
    void setUIParams(){
        
        defaultSimpleGuiConfig.gridSize.x = 351;
        
        gui.addToggle("8 Connected", connect8 );
        gui.addToggle("use sqrtf", routing->useSqrt );
        
        gui.addSlider("Eucledian Dist weight", routing->eucledianDistWeight, 0.0, 1.0);
        gui.addSlider("road Cost Gain", routing->roadCostGain, 0, 300);
        gui.addSlider("extra cost per step", routing->perStepExtraCost, -1, 1);
        gui.addSlider("max Iterations", maxIterations, 100, 100000);
        
        gui.loadFromXML();
    };
#endif
    
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void reloadImage();
    //void processOpenFileSelection(ofFileDialogResult openFileResult);

    //-Map Image
    string originalFileExtension;
    ofxSimpleGuiToo gui;
    
    //-Model
    int playModel;
    
    //-Settings
    ofXml* playSettings;
    ofEsSettings* settings;
    unsigned int  settingAcount;
    
    
    
};
