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
    lineCounter = 0;
    tmpR = 0;
    tmpG = 0;
    tmpB = 0;
    tmpC = 0;
    
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
    if (camera.isFrameNew()){
        
        pixels = camera.getPixels();
        int totalPixels = camWidth * camHeight;
        lineCounter = 0;
        unsigned char tmpR = 0;
        unsigned char tmpG = 0;
        unsigned char tmpB = 0;
        unsigned char tmpC = 0;
        
        for (int i = 0; i < totalPixels; i++) {
            
            // Adding Colors
            tmpR += pixels[i];
            tmpG += pixels[i+1];
            tmpB += pixels[i+2];
            //tmpC += pixels[i];
            
            // Store Color
            if(i % camWidth == 0) {
                //cout << "TEMP CEE FOR LINE #" << lineCounter << " IS " << color << endl;
                
                // Set Avg Colours To Color Array
                lineColors[lineCounter].r = int(tmpR);
                lineColors[lineCounter].g = int(tmpG);
                lineColors[lineCounter].b = int(tmpB);
                
                // Reset Temp Colors
                tmpR = 0;
                tmpG = 0;
                tmpB = 0;
                //tmpC = 0;
                
                // Iterate
                lineCounter++;
                
            }
        }
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Raw Camera
    ofSetColor(255);
    camera.draw(50, 50, camWidth, camHeight);
    
    // Lines
    for (int i = 0; i < camHeight; i++) {
        ofSetColor(lineColors[i]);
        //cout << lineColors[i] << endl;
        ofLine(camWidth + 100, 50 + i, camWidth*2 + 100, 50 + i);
    }
    
    
    // Debug
    ofSetColor(0);
    char fpsStr[255];
    sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
    ofDrawBitmapString(fpsStr, 50, 700);

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
