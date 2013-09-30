#pragma once

#include "ofMain.h"
#include "AppCore.h"

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
    
        // Pd
        AppCore core;
    
        // Camera
        ofVideoGrabber  camera;
        int camWidth, camHeight;
        unsigned char *pixels;
        unsigned char tmpR;
        unsigned char tmpG;
        unsigned char tmpB;
        unsigned char tmpC;
        int lineCounter;
        ofColor lineColors[480];
        ofColor tempColor;
    
        unsigned char *videoInverted;
        ofTexture videoTexture;
    
        // Font
        ofTrueTypeFont font;
    
        // Misc
        Boolean verbose;
    
    
};
