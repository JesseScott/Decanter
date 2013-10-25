#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup() {
    
    // Screen
    ofSetWindowTitle("beeeeeeeer");
    ofBackground(255);
    
    // Font
    font.loadFont("font/Courier New Bold.ttf", 40);
    
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
            ofSetBackgroundColor(255);
            for(int i = 0; i < camHeight; i++) {
                if(i % 10 == 0) {
                    ofSetColor(lineColors[i].r, 0, 0);
                    char r = lineColors[i].r;
                    font.drawString(ofToString(r), (i*2) + 15, lineCounter/5);
                    
                    ofSetColor(0, lineColors[i].g, 0);
                    char g = lineColors[i].g;
                    font.drawString(ofToString(g), (i*2) + 15, 150 + lineCounter/5);
                    
                    ofSetColor(0, 0, lineColors[i].b);
                    char b = lineColors[i].b;
                    font.drawString(ofToString(b), (i*2) + 15, 300 + lineCounter/5);
                }
            }
        dataSet.end();
     
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
    
    ofSetColor(255);
    
    // Raw Camera
    camera.draw(0, 0, cellWidth, cellHeight); // 0, 0  || TL
    
    // Average Colour Lines
    averageLines.draw(cellWidth, 0, cellWidth, cellHeight); // 0, 0    || TC

    // Sorted Colour Lines
    sortedLines.draw(cellWidth*2, 0, cellWidth, cellHeight); // 960, 0    || TR
    
    // Data Set
    dataSet.draw(0, cellHeight, cellWidth, cellHeight); // 0, 360   || ML
    
    // Cropped Camera
    //croppedCamera.draw(0, cellHeight, cellWidth, cellHeight); // 0, 360    || ML
    
    // Texture
    //tex.draw(cellWidth, cellHeight, cellWidth, cellHeight); // 480, 360    || MC
     
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

void ofApp::exit() {
    ofLogNotice("Exiting App");
    
    // Close Camera
    camera.close();
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
