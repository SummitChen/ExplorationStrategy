#include "ofApp.h"
#include "ofxXmlSettings.h"
#include "esBotReModel.h"
#include <cstdlib>


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofEnableAlphaBlending();
	ofSetFrameRate(60);
    
    settings = new ofEsSettings();
    settings->load("Settings.XML");
    
    settingAcount = 0;
    
    if ( settings->getConfigurationSet().size() != 0) {
        playModel = settings->getConfigurationSet()[settingAcount].playModel;
    }
    
    switch (playModel) {
        case BOT:
            esBotModel::getInstance()->initialize(settings->getConfigurationSet()[settingAcount]);
            break;
        case BOT_REPLAY:
            esBotReModel::getInstance()->initialize(settings->getConfigurationSet()[settingAcount]);
            break;
        default:
            std::exit(1);
            break;
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    bool simulationEnd = false;
    
    switch (playModel) {
        case BOT:
            esBotModel::getInstance()->updateStatus();
            simulationEnd = esBotModel::getInstance()->isSimulationEnd();
            if ( simulationEnd ) {
                esBotReModel::destroyInstance();
            }
            break;
        case BOT_REPLAY:
            esBotReModel::getInstance()->updateStatus();
            simulationEnd = esBotModel::getInstance()->isSimulationEnd();
            if ( simulationEnd) {
                esBotReModel::destroyInstance();
            }
            break;
        default:
            std::exit(1);
            break;
    }
    
    if ( simulationEnd ) {
        
        if ( settingAcount < settings->getConfigurationSet().size() - 1 ) {
            settingAcount ++;
        }else{
            std::exit(1);
        }

        playModel = settings->getConfigurationSet()[settingAcount].playModel;
        
        switch (playModel) {
            case BOT:
                esBotModel::getInstance()->initialize(settings->getConfigurationSet()[settingAcount]);
                break;
            case BOT_REPLAY:
                esBotReModel::getInstance()->initialize(settings->getConfigurationSet()[settingAcount]);
                break;
            default:
                std::exit(1);
                break;
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    switch (playModel) {
        case BOT:
            esBotModel::getInstance()->rendering();
            break;
        case BOT_REPLAY:
            esBotReModel::getInstance()->rendering();
            break;
        default:
            std::exit(1);
            break;
    }

    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::mousePressed(int x, int y, int button){

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    /*
     if (key == '\t') gui.toggleDraw();
     
     if (key == ' ') ofToggleFullscreen();
     
     if (key == 'o') {
     
     //Open the Open File Dialog
     ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");
     
     //Check if the user opened a file
     if (openFileResult.bSuccess){
     
     ofLogVerbose("User selected a file");
     
     //We have a file, check it and process it
     processOpenFileSelection(openFileResult);
     
     }else {
     ofLogVerbose("User hit cancel");
     }
     }
     */
    
    if ( key == 's') {
        if ( playModel == BOT) {
            esBotModel::getInstance()->saveLog();
        }
    }
}

//------------------------------------------------------------
/*
 void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
 
 ofLogVerbose("getName(): "  + openFileResult.getName());
 ofLogVerbose("getPath(): "  + openFileResult.getPath());
 
 ofFile file (openFileResult.getPath());
 
 if (file.exists()){
 
 ofLogVerbose("The file exists - now checking the type via file extension");
 string fileExtension = ofToUpper(file.getExtension());
 
 //We only want images
 if (fileExtension == "JPG" || fileExtension == "PNG") {
 
 //Save the file extension to use when we save out
 originalFileExtension = fileExtension;
 
 //printf("%s", openFileResult.getPath().c_str());
 //cout<< openFileResult.getPath()<<std::endl;
 
 delete routing;
 delete grid_map;
 delete map;
 
 routing = NULL;
 grid_map = NULL;
 map = NULL;
 
 
 ofSetLogLevel(OF_LOG_VERBOSE);
 ofEnableAlphaBlending();
 ofSetFrameRate(60);
 
 map = new ofxTileMap(openFileResult.getName());
 grid_map = new ofxTileMap(map->getWidth(), map->getHeight());
 routing = new ofxMapRouting( map, false );
 
 ofSetWindowShape(map->getWidth(), map->getHeight());
 ofSetWindowPosition(12, 12);
 
 origin.x = map->getWidth()/2;
 origin.y = map->getHeight()/2 - 5;
 
 while ( map->getTileSafe( origin.x, origin.y) != WALKABLE) {
 origin.x = rand() % map->getWidth();
 origin.y = rand() % map->getHeight();
 }
 
 scout = Robot( origin, 1.0, 40);
 reset_original = true;
 
 update_map_status();
 select_next_pos();
 
 }
 }
 }
 */
