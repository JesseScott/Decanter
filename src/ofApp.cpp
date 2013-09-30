#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Pd
    int ticksPerBuffer = 8;
    ofSoundStreamSetup(2, 1, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
    core.setup(2, 1, 44100, ticksPerBuffer);
    
    // Camera
    camWidth = 640;
    camHeight = 480;
    camera.setVerbose(true);
    camera.initGrabber(camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::update(){

    // Camera
    camera.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Raw Camera
    camera.draw(50, 50, camWidth, camHeight);
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mousePressed(int x, int y, int button){

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
