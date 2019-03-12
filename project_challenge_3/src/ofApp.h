#pragma once

#include "Piece.hpp"
#include "Collage.hpp"
#include "ofMain.h"
#include "MusicButton.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        ofVideoGrabber cam;
        int camWidth;
        int camHeight;
        ofPixels video_pixels;
        ofPixels previous_pixels;

        ofImage buffer;
        int midX = 0;
        int midY = 0;
    
        bool touching = false;
        bool holding = false;
    
        int state = 0;
        vector<ofPoint> vec;
        vector<MusicButton> buttons;
    
        float threshold_color;
        float threshold;
        float avgX;
        float avgY;
        int points;
    
        int count;

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
        void checkPiece(ofVec2f cursor);
		
    
        string bgPath = "bg.jpg";
        vector<string> pathes{"red.jpg","blue.jpg","black.jpg","yellow.jpg"};
        vector<ofVec2f> posVec{
            ofVec2f(412,137),ofVec2f(721,518),
            ofVec2f(415,441),ofVec2f(718,137) };
        vector<ofVec2f> sizeVec{
            ofVec2f(296,295),ofVec2f(181,107),
            ofVec2f(146,143),ofVec2f(182,142) };
    
        Collage testCollage;
        Piece* touchedPiece;
    
    void updateState2();
    void updateState1();
    
    void drawState1();
    void drawState2();
};
