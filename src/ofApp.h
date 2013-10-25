#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        // Camera
        ofVideoGrabber  camera;
        int camWidth, camHeight;
        unsigned char *cameraPixels;
        unsigned char *colorPixels;
    
        // Crop
        int cropWidth, cropOffset;
        int nearBeer, farBeer;
        ofImage croppedCamera;
        ofPixels tmpCamera;
    
        // Syphon
        ofTexture tex;
        ofFbo texFbo;
        ofPixels pixelArray;
        ofxSyphonServer mainOutputSyphonServer;
        ofxSyphonServer individualTextureSyphonServer;
        ofxSyphonClient mClient;
        bool drawAvgLines;
    
        // Colour
        int tmpR;
        int tmpG;
        int tmpB;
        int lineCounter;
        ofColor lineColors[480];
        ofColor blockColors[48];
    
        // FBOs
        ofFbo   averageLines;
        ofFbo   sortedLines;
        ofFbo   dataSet;
    
        // Font
        ofTrueTypeFont font;
    
        // Misc
        Boolean verbose;
        int cellWidth, cellHeight;
    
    
};
