#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"

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
    
    ofxCvColorImage            colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;
    ofxCvContourFinder     contourFinder;
    ofSoundPlayer           dub;
    
    int                 threshold;
    bool                bLearnBakground;
    bool                bShowVideo = true;
    
    ofVideoGrabber         vidGrabber;
    ofxOscReceiver         receiver;
    int nearThreshold;
    int farThreshold;
    
    float posX;
    float posY;
    float vol;
    
    vector<glm::vec3> points;
    
    ofColor colors[52][39];
    
    ofColor color_set[5];
 
    ofColor bg = ofColor(203,143,144);
    
    int number_of_colors = 6;
};

