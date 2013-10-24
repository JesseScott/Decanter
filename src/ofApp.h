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
        unsigned char *pixels;
    
        // Video Recorder
        ofVideoGrabber 			vidGrabber;
        ofPtr<ofQTKitGrabber>	vidRecorder;
        void videoSaved(ofVideoSavedEventArgs& e);
        vector<string> videoDevices;
        vector<string> audioDevices;
        
        bool bLaunchInQuicktime;
    
        // Syphon
        ofTexture tex;
        ofxSyphonServer mainOutputSyphonServer;
        ofxSyphonServer individualTextureSyphonServer;
        ofxSyphonClient mClient;
    
        // Colour
        int tmpR;
        int tmpG;
        int tmpB;
        int lineCounter;
        ofColor lineColors[480];
        ofColor blockColors[48];
        ofColor tempColor, currentColor, lastColor;
        int currentColorInt[3];
        int lastColorInt[3];
        int colorDifference;
    
        // FBOs
        ofFbo   averageLines;
        ofFbo   averageBlocks;
    
        // Font
        ofTrueTypeFont font;
    
        // Misc
        Boolean verbose;
    
    
};
