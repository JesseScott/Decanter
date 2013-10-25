#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup() {
    
    // Screen
    ofSetWindowTitle("beeeeeeeer");
    ofBackground(255);
    
    // Font
    font.loadFont("font/Courier New Bold.ttf", 9);
    
    // Misc
    verbose = true;
    cellWidth  = 480;
    cellHeight = 320;
    
    // Camera
    camWidth = 640;
    camHeight = 480;
    cropWidth = camWidth/2;
    cropOffset = camWidth/4;
    lineCounter = 0;
    
    tmpR = 0;
    tmpG = 0;
    tmpB = 0;
    colorDifference = 25;
    currentColorInt[0] = 0; currentColorInt[1] = 0; currentColorInt[2] = 0;
    lastColorInt[0] = 0; lastColorInt[1] = 0; lastColorInt[2] = 0;    
    
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
    
    croppedCamera.allocate(camWidth, camHeight, OF_IMAGE_COLOR_ALPHA);
     
    // FBOs
    averageLines.allocate(camWidth, camHeight, GL_RGBA);
    averageLines.begin();
        ofClear(255,255,255, 0);
    averageLines.end();
    
    averageBlocks.allocate(camWidth, camHeight, GL_RGBA);
    averageBlocks.begin();
        ofClear(255,255,255, 0);
    averageBlocks.end();
    
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
        
        pixels = camera.getPixels();
        int totalPixels = camWidth * camHeight;
        lineCounter = 0;
        bool startAdding = false;
        
        // Get Average Colours
        for (int i = 0; i < totalPixels; i++) {
            
            if(i % cropOffset == 0) {
                startAdding = true;
            }

            if(i % (camWidth - cropOffset) == 0) {
                startAdding = false;
            }

            //cout << ofToString(startAdding);
            
            // Adding Colors
            if(startAdding) {
                tmpR += pixels[i*3];
                tmpG += pixels[i*3+1];
                tmpB += pixels[i*3+2];
            }
            
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
                
                //colorPixels[i*3]   = lineColors[lineCounter].r;
                //colorPixels[i*3+1] = lineColors[lineCounter].g;
                //colorPixels[i*3+2] = lineColors[lineCounter].g;
                
                // Add Averages
                tmpR += tmpR;
                tmpG += tmpG;
                tmpB += tmpB;
                
                // Set Block Averages
                if(lineCounter % 10 == 0) {
                    blockColors[lineCounter/10].r = tmpR;
                    blockColors[lineCounter/10].g = tmpG;
                    blockColors[lineCounter/10].b = tmpB;
                    
                    currentColorInt[0] = tmpR;
                    currentColorInt[1] = tmpG;
                    currentColorInt[2] = tmpB;
                }
                 
                // Test Colour Difference
                if( (currentColorInt[0] - lastColorInt[0]) < colorDifference ||
                    (currentColorInt[1] - lastColorInt[1]) < colorDifference ||
                    (currentColorInt[2] - lastColorInt[2]) < colorDifference ) {
                    
                }
                else {
                
                }
                
                // Store Last Color
                lastColorInt[0] = currentColorInt[0];
                lastColorInt[1] = currentColorInt[1];
                lastColorInt[2] = currentColorInt[2];
                
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
            ofClear(128, 128, 128, 255); // --->>> CALIBRATE
            for(int i = 0; i < camHeight; i++) {
                ofSetColor(lineColors[i]);
                ofLine(0, i, camWidth, i);
            }
        averageLines.end();
        
        averageBlocks.begin();
            for(int i = 0; i < camHeight/10; i++) {
                ofSetColor(blockColors[i]);
                ofRect(0, 0 + i*10, camWidth, 0 + i*10);
            }
        averageBlocks.end();
     
        // Texture For Syphon
        averageLines.readToPixels(pixelArray);
        colorPixels =  pixelArray.getPixels();
        tex.loadData(colorPixels, 640, 480, GL_RGBA);
        
	}
    
}

//--------------------------------------------------------------

void ofApp::draw() {
    
    // Raw Camera
    ofSetColor(255);
    camera.draw(0, 0, cellWidth, cellHeight);
    
    // Cropped Camera
    
    // Average Colour Lines
    ofSetColor(255);
    averageLines.draw(cellWidth, 0, cellWidth, cellHeight);

    // Block Colour Lines
    ofSetColor(255);
    averageBlocks.draw(cellWidth*2, 0, cellWidth, cellHeight);
    
    // Texture
    ofSetColor(255, 255, 255);
    //tex.draw(camWidth, 0, cellWidth, cellHeight);
     
    // Syphon
	mainOutputSyphonServer.publishScreen();
    individualTextureSyphonServer.publishTexture(&tex);
     
    // Debug
    ofSetColor(0);
    char fpsStr[255];
    sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
    ofDrawBitmapString(fpsStr, 50, ofGetWindowHeight() - 50);
    
}

//--------------------------------------------------------------

void ofApp::exit() {
    ofLogNotice("Exiting App");
    
    // Close Camera
    camera.close();
}


//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    
    // Camera Settings
    if (key == 's' || key == 'S') {
		//camera.videoSettings();
        ofSaveFrame();
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
