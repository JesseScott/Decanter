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
    int r = 0; int g = 0; int b = 0; int a = 0;

    if (camera.isFrameNew()){
        
        ofPixels pixels = camera.getPixelsRef();
        int count = 0;
        
        for(int x = 0; x < pixels.getWidth(); x += 4) {
            for(int y = 0; y < pixels.getHeight(); y += 4) {
                ofColor c = pixels.getColor(x, y);
                r += c.r;
                g += c.g;
                b += c.b;
                a += c.a;
                count++;
            }
        }
        
        cout << "PRE    R : " << r << " G : " << g << " B : " << b << endl;
        int totalPixels = camWidth * camHeight;
        r = r / count;
        g = g / count;
        b = b / count;
        cout << "FINAL    R : " << r << " G : " << g << " B : " << b << endl;
        
        /*
        int totalPixels = camWidth*camHeight*3;
        unsigned char *pixels = camera.getPixels();
        for (int i = 0; i < totalPixels-1; i++) {
            
            r += pixels[i * 3];
            g += pixels[i * 3 + 1];
            b += pixels[i * 3 + 2];
            
        }
        cout << "PRE    R : " << r << " G : " << g << " B : " << b << endl;
        r = r / totalPixels;
        g = g / totalPixels;
        b = b / totalPixels;
        cout << "TL : " << totalPixels << endl;
        cout << "FINAL    R : " << r << " G : " << g << " B : " << b << endl;
        */
        
	}
    
    ofBackground(r,g,b);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Raw Camera
    camera.draw(50, 50, camWidth, camHeight);
    
    // Debug
    ofSetColor(255);
    char fpsStr[255];
    sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
    font.drawString(fpsStr, 50, 700);

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
