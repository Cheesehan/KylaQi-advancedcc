#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGif.h"

#define PORT1 8000
#define PORT2 9000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //pixel function
    void pixel(int x, int y);
    void pixelw(int x, int y);
    int edge = 0;
    
    //fire function
    void fire();
    
    //wave function
    void wave();
		
    float x;
    float y;
    float z;
    
    float q;
    float w;
    float e;
    
    vector<glm::vec3> points;
    ofxOscReceiver receiver1;
    ofxOscReceiver receiver2;
    
    //gif loader
    int m=0 ;
    int lasttime = ofGetElapsedTimef();
    ofxGIF::fiGifLoader gifloader1;
    ofxGIF::fiGifLoader gifloader2;
    
    
    //circle
    int xLoc = 100;
    int yLoc = 400;
    
};
