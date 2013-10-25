#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup() {
    
    // Screen
    ofSetWindowTitle("beeeeeeeer");
    ofBackground(255);
    
    // Font
    mainFont.loadFont("font/Ostrich.ttf", 40);
    subFont.loadFont("font/Ostrich.ttf", 25);
    
    // Misc
    verbose = false;
    cellWidth  = 480;
    cellHeight = 360;
    
    // Camera
    camWidth = 640;
    camHeight = 480;
    cropWidth = camWidth/2;
    cropOffset = camWidth/4;
    nearBeer = 160;
    farBeer = 320;
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
            }
            else {
                cout << " - unavailable " << endl;
            }
        }
    }
    
	camera.setDeviceID(0);
	camera.setDesiredFrameRate(60);
    camera.setVerbose(true);
    camera.initGrabber(camWidth, camHeight);
    
    croppedCamera.allocate(cropWidth, camHeight, OF_IMAGE_COLOR_ALPHA);
    
    // Audio
    int bufferSize = 256;
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(camWidth, 0.0);
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    soundStream.start();
     
    // FBOs
    averageLines.allocate(camWidth, camHeight, GL_RGBA);
    averageLines.begin();
        ofClear(255,255,255, 0);
    averageLines.end();
    
    sortedLines.allocate(camWidth, camHeight, GL_RGBA);
    sortedLines.begin();
        ofClear(255,255,255, 0);
    sortedLines.end();
    
    dataSet.allocate(camWidth, camHeight, GL_RGBA);
    dataSet.begin();
        ofClear(255,255,255, 0);
    dataSet.end();
    
    interpretivePanel.allocate(camWidth, camHeight, GL_RGBA);
    interpretivePanel.begin();
        ofClear(255,255,255, 0);
    interpretivePanel.end();
    
    audioFbo.allocate(camWidth, camHeight, GL_RGBA);
    audioFbo.begin();
        ofClear(255,255,255, 0);
    audioFbo.end();
    
    drawAvgLines = true;
    
    // Syphon
	mainOutputSyphonServer.setName("Screen Output");
	individualTextureSyphonServer.setName("Texture Output");
	mClient.setup();
    mClient.set("","Simple Server");
	

    tex.allocate(camWidth, camHeight, GL_RGBA);
    pixelArray.allocate(camWidth, camHeight, OF_PIXELS_RGBA);
    colorPixels = new unsigned char[640*480*4];
    
    cout << " -- END OF SETUP -- " << endl;
}

//--------------------------------------------------------------

void ofApp::update() {
    
    // Camera
    camera.update();
    if (camera.isFrameNew()){
        
        // Get Camera Pixels
        cameraPixels = camera.getPixels();
        
        // Pull Cam Pix & Crop
        tmpCamera.setFromPixels(cameraPixels, camWidth, camHeight, OF_IMAGE_COLOR);
        croppedCamera.setFromPixels(tmpCamera);
        croppedCamera.crop(cropOffset, 0, camWidth - cropWidth, camHeight);
        croppedCamera.resize(camWidth, camHeight);
        
        // Set CameraPix from Cropped Image
        cameraPixels = croppedCamera.getPixels();
        
        int totalPixels = camWidth * camHeight;
        lineCounter = 0;
        bool startAdding = false;
        
        // Get Average Colours
        for (int i = 0; i < totalPixels; i++) {
            
            // Adding Colors
            tmpR += cameraPixels[i*3];
            tmpG += cameraPixels[i*3+1];
            tmpB += cameraPixels[i*3+2];
            
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
        
        // Audio
        scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        volHistory.push_back( scaledVol );
        if( volHistory.size() >= 400 ){
            volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }
        
        // Draw FBOs
        averageLines.begin();
            ofClear(128, 128, 128, 255);
            for(int i = 0; i < camHeight; i++) {
                ofSetColor(lineColors[i]);
                ofLine(0, i, camWidth, i);
            }
        averageLines.end();
        
        sortedLines.begin();
            ofClear(128, 128, 128, 255);
            for(int i = 0; i < camHeight/10; i++) {
                ofSetColor(blockColors[i]);
                ofRect(0, -10 + i*10, camWidth, -10 + i*10);
            }
        sortedLines.end();
        
        dataSet.begin();
            ofClear(0, 0, 0, 5);
            ofSetBackgroundColor(0);
            for(int i = 0; i < camHeight; i++) {
                if(i % 10 == 0) {
                    ofSetColor(lineColors[i].r, 0, 0);
                    char r = lineColors[i].r;
                    mainFont.drawString(ofToString(r), (i*2) + 15, lineCounter/5);
                    
                    ofSetColor(0, lineColors[i].g, 0);
                    char g = lineColors[i].g;
                    mainFont.drawString(ofToString(g), (i*2) + 15, 150 + lineCounter/5);
                    
                    ofSetColor(0, 0, lineColors[i].b);
                    char b = lineColors[i].b;
                    mainFont.drawString(ofToString(b), (i*2) + 15, 300 + lineCounter/5);
                }
            }
        dataSet.end();
        
        interpretivePanel.begin();
            ofClear(0, 0, 0, 255);
            ofSetBackgroundColor(0);
        
            ofSetColor(255);
            string title = "DECANTER";
            mainFont.drawString(title, camWidth/2 - 85, camHeight/2 - 50);
        
            ofSetColor(200);
            string subtitle = "- a generative audio alcoholic experience -";
            subFont.drawString(subtitle, camWidth/4 - 75, camHeight/2);
        interpretivePanel.end();
        
        audioFbo.begin();
            ofClear(0, 0, 0, 255);
            ofSetLineWidth(3);
            ofSetColor(245, 58, 135);
        
            ofBeginShape();
                for (unsigned int i = 0; i < left.size(); i++){
                    ofVertex(i*3, 100 -left[i]*180.0f);
                }
            ofEndShape(false);

            ofBeginShape();
                for (unsigned int i = 0; i < right.size(); i++){
                    ofVertex(i*3, 200 -right[i]*180.0f);
                }
            ofEndShape(false);
        
            ofBeginShape();
            for (unsigned int i = 0; i < volHistory.size(); i++){
                if( i == 0 ) ofVertex(i, camHeight);
                ofVertex(i, camHeight - volHistory[i] * 150);
                if( i == volHistory.size() -1 ) ofVertex(i, camHeight);
            }
            ofEndShape(false);
        
        audioFbo.end();
     
        // Texture For Syphon
        if(drawAvgLines) {
            averageLines.readToPixels(pixelArray);
        }
        else {
            sortedLines.readToPixels(pixelArray);
        }
        colorPixels =  pixelArray.getPixels();
        tex.loadData(colorPixels, 640, 480, GL_RGBA);
        
	}
    
}

//--------------------------------------------------------------

void ofApp::draw() {
    
    // Raw Camera
    ofSetColor(255);
    camera.draw(0, 0, cellWidth, cellHeight); // 0, 0  || TL
    
    // Average Colour Lines
    ofSetColor(255);
    averageLines.draw(cellWidth, 0, cellWidth, cellHeight); // 0, 0    || TC

    // Sorted Colour Lines
    ofSetColor(255);
    sortedLines.draw(cellWidth*2, 0, cellWidth, cellHeight); // 960, 0    || TR
    
    // Data Set
    ofSetColor(255);
    dataSet.draw(0, cellHeight, cellWidth, cellHeight); // 0, 360   || ML
    
    // Interpretive Text
    ofSetColor(255);
    interpretivePanel.draw(cellWidth, cellHeight, cellWidth, cellHeight); // 360, 360   || MC
    
    // Audio Waverform
    audioFbo.draw(cellWidth*2, cellHeight, cellWidth, cellHeight);
    
    // Cropped Camera
    //croppedCamera.draw(0, cellHeight, cellWidth, cellHeight); // 0, 360    || ML
    
    // Texture
    //tex.draw(cellWidth, cellHeight, cellWidth, cellHeight); // 480, 360    || MC
    
//    ofSetColor(255, 0, 0);
//    ofLine(ofGetWidth()/2, 0, ofGetWidth()/2, 720);
    
    // Syphon
	mainOutputSyphonServer.publishScreen();
    individualTextureSyphonServer.publishTexture(&tex);
     
    // Debug
    if(verbose) {
        ofSetColor(0);
        char fpsStr[255];
        sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
        ofDrawBitmapString(fpsStr, 50, ofGetWindowHeight() - 50);
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

	float curVol = 0.0;
	int numCounted = 0;
    
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	curVol /= (float)numCounted;
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}

//--------------------------------------------------------------

void ofApp::exit() {
    ofLogNotice("Exiting App");
    
    // Close Camera
    camera.close();
    
    // Close Audio
    soundStream.stop();
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    
    // Camera Settings
    if (key == 's' || key == 'S') {
		camera.videoSettings();
        //ofSaveFrame();
	}

    // FBO -> Syphon
    if (key == 'f' || key == 'F') {
		drawAvgLines = !drawAvgLines;
        cout << "DAL = " << ofToString(drawAvgLines) << endl;
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
