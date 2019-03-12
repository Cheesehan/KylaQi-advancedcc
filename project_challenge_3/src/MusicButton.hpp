//
//  MusicButton.hpp
//  assign3
//
//  Created by wenhe on 2019/3/8.
//

#ifndef MusicButton_hpp
#define MusicButton_hpp

#include "ofMain.h"

class MusicButton {
public:
    MusicButton();
    
    ofVec2f pos;
    ofSoundPlayer sound;
    float r;
    float ori_r;
    ofColor c;
    bool tri;
    
    void setup(float x, float y, ofColor _c, string src);
    void update(vector<ofPoint> vec);
    void draw();
    
private:
    void wave();
    
};

#endif /* MusicButton_hpp */

