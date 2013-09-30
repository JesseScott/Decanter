#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Screen
    ofBackground(255);
    
    // Font
    font.loadFont("font/Courier New Bold.ttf", 9);
    
    // Misc
    verbose = true;
    
    // Pd
    int ticksPerBuffer = 8;
    ofSoundStreamSetup(2, 1, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
    core.setup(2, 1, 44100, ticksPerBuffer);
    
    // Camera
    camWidth = 640;
    camHeight = 480;
    vector<ofVideoDevice> devices = camera.listDevices();
	if(verbose) {
        for(int i = 0; i < devices.size(); i++){
            cout << devices[i].id << ": " << devices[i].deviceName;
            if( devices[i].bAvailable ){
                cout << endl;
            }else{
                cout << " - unavailable " << endl;
            }
        }
    }
	camera.setDeviceID(4);
	camera.setDesiredFrameRate(60);
    camera.setVerbose(true);
    camera.initGrabber(camWidth, camHeight);

    cout << " -- END OF SETUP -- " << endl;
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
void ofApp::exit() {
    ofLogNotice("Exiting App");
    
    // Close Pd
	core.exit();
    
    // Close Camera
    camera.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    // Camera Settings
    if (key == 's' || key == 'S'){
		camera.videoSettings();
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
