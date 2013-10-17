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
    
    /*  vector<ofVideoDevice> devices = camera.listDevices();
     if(verbose) {
     for(int i = 0; i < devices.size(); i++){
     cout << devices[i].id << ": " << devices[i].deviceName;
     if( devices[i].bAvailable ){
     cout << endl;
     }else{
     cout << " - unavailable " << endl;
     }
     }
     }*/
	camera.setDeviceID(0);
	camera.setDesiredFrameRate(60);
    camera.setVerbose(true);
    camera.initGrabber(camWidth, camHeight);
    
    videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);
    
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
        int tmpR = 0;
        int tmpG = 0;
        int tmpB = 0;
        
        int tempCounter = 0;
        
        for (int i = 0; i < totalPixels; i++) {
            
            //  videoInverted[i] = 255 - pixels[i];
            
            // Adding Colors
            tmpR += pixels[i*3];
            tmpG += pixels[i*3+1];
            tmpB += pixels[i*3+2];
            //tmpC += pixels[i];
            tempCounter++;
            // Store Color
            if(i % camWidth == 0) {
                // get the average value
                
                ofLogNotice(ofToString(totalPixels));
                ofLogNotice(ofToString(tmpR));
                tmpR = tmpR/camWidth;
                tmpG = tmpG/camWidth;
                tmpB = tmpB/camWidth;
                ofLogNotice("tempR = " + ofToString(tmpR));
                // Set Avg Colours To Color Array
                lineColors[lineCounter].r = int(tmpR);
                lineColors[lineCounter].g = int(tmpG);
                lineColors[lineCounter].b = int(tmpB);
                
                // Reset Temp Colors
                tmpR = 0;
                tmpG = 0;
                tmpB = 0;
                
                // Iterate
                lineCounter++;
                
            }
        }
        // videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Raw Camera
    ofSetColor(255);
    camera.draw(50, 50, camWidth, camHeight);
    // videoTexture.draw(50 ,camHeight + 50, camWidth, camHeight);
    
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
