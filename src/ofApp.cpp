#include "ofApp.h"
#include "ofxXmlSettings.h"
#include "esBotReModel.h"
#include <cstdlib>


//-------------------------------------------------------------

ofApp::ofApp(){}

ofApp::~ofApp(){
    
    if (settings != NULL) {
        delete settings;
        settings = NULL;
    }
}
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
        
        switch (playModel) {
            case BOT:
                esBotModel::getInstance()->destroyInstance();
                break;
            case BOT_REPLAY:
                esBotReModel::getInstance()->destroyInstance();
                break;
            default:
                std::exit(1);
            break;
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

