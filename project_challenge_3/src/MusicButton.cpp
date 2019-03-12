//
//  MusicButton.cpp
//  assign3
//
//  Created by wenhe on 2019/3/8.
//

#include "MusicButton.hpp"


MusicButton::MusicButton(){}

void MusicButton::setup(float x, float y, ofColor _c, string src) {
    pos = ofVec2f(x, y);
    c = _c;
    r = 50;
    ori_r = 50;
    sound.load(src);
    tri = false;
}

void MusicButton::update(vector<ofPoint> vec) {
    int count= 0;
    for (auto &v : vec) {
        if (v.distance(pos) < r) {
            count ++;
        }
    }
    
    if (count > 5 && !sound.isPlaying() && !tri) {
        sound.play();
        tri = true;
    }
}


void MusicButton::draw() {
    if (!tri) {
        ofPushStyle();
        ofSetColor(c);
        ofDrawCircle(pos, r);
        ofPopStyle();
    } else {
        ofDrawCircle(pos, ori_r);
        ofPushStyle();
        r = ofLerp(r, 2.5*r, .1);
        ofSetColor(c, ofMap(r, 50, 100, 0, 255));
        ofDrawCircle(pos, r);
        ofPopStyle();
        if (r >= ori_r * 2) {
            tri = false;
            r = ori_r;
        }
    }
}

