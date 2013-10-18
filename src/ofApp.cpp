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
    
    // Audio
    soundStream.listDevices();

	int bufferSize = 256;
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    // Camera
    camWidth = 640;
    camHeight = 480;
    lineCounter = 0;
    
    tmpR = 0;
    tmpG = 0;
    tmpB = 0;
    
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
    
    // FBOs
    averageLines.allocate(camWidth, camHeight, GL_RGBA);
    averageLines.begin();
        ofClear(255,255,255, 0);
    averageLines.end();
    
    averageBlocks.allocate(camWidth, camHeight, GL_RGBA);
    averageBlocks.begin();
        ofClear(255,255,255, 0);
    averageBlocks.end();
    
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
        int tempCounter = 0;
        
        // Get Average Colours
        for (int i = 0; i < totalPixels; i++) {
            // Adding Colors
            tmpR += pixels[i*3];
            tmpG += pixels[i*3+1];
            tmpB += pixels[i*3+2];
            tempCounter++;
            
            // Store Color
            if(i % camWidth == 0) {
                // get the average value
                tmpR = tmpR / camWidth;
                tmpG = tmpG / camWidth;
                tmpB = tmpB / camWidth;

                // Set Avg Colours To Color Array
                lineColors[lineCounter].r = tmpR;
                lineColors[lineCounter].g = tmpG;
                lineColors[lineCounter].b = tmpB;
                
                // Add Averages
                tmpR += tmpR;
                tmpG += tmpG;
                tmpB += tmpB;
                
                // Set Block Averages
                if(lineCounter % 10 == 0) {
                    blockColors[lineCounter/10].r = tmpR;
                    blockColors[lineCounter/10].g = tmpG;
                    blockColors[lineCounter/10].b = tmpB;
                }
                
                // Reset Temp Colors
                tmpR = 0;
                tmpG = 0;
                tmpB = 0;
                
                // Iterate
                lineCounter++;
            }
        }
        
        // Draw FBOs
        averageLines.begin();
            for(int i = 0; i < camHeight; i++) {
                ofSetColor(lineColors[i]);
                ofLine(0, 0 + i, camWidth, 0 + i);
            }
        averageLines.end();
        
        averageBlocks.begin();
            for(int i = 0; i < camHeight/10; i++) {
                ofSetColor(blockColors[i]);
                ofRect(0, 0 + i*10, camWidth, 0 + i*10);
            }
        averageBlocks.end();
        
	}
    
}

//--------------------------------------------------------------

void ofApp::draw(){
    
    // Raw Camera
    ofSetColor(255);
    camera.draw(0, 0, camWidth, camHeight);
    
    // Average Colour Lines
    ofSetColor(255);
    averageLines.draw(camWidth, 0, camWidth, camHeight);

    // Block Colour Lines
    ofSetColor(255);
    averageBlocks.draw(camWidth*2, 0, camWidth, camHeight);
    
    // Debug
    ofSetColor(0);
    char fpsStr[255];
    sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
    ofDrawBitmapString(fpsStr, 50, ofGetWindowHeight() - 50);
    
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

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
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
