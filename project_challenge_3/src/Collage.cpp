//
//  Collage.cpp
//  project_challenge_3
//
//  Created by Casey Pan on 3/8/19.
//

#include "Collage.hpp"


Collage::Collage(){
    ofLogNotice() << "constructed a Collage" << endl;
}



Collage::Collage(string bg_path, vector<string> pathes,vector<ofVec2f> posVec, vector<ofVec2f> sizeVec){
    
    bg.load(bg_path);
    
    FILE_PATHES = pathes;
    poses = posVec;
    sizes = sizeVec;
    
    for(int i=0; i<pathes.size(); i++){
        pieces.push_back( Piece( pathes[i], poses[i], sizes[i] ) );
    }
}



void Collage::update(ofVec2f cursor){
    for (auto &p : pieces) {
        p.update(cursor);
    }
}



void Collage::draw(){
    // draw the background
    bg.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // draw all pieces of this collage
    for(Piece p : pieces){
        p.draw();
    }
}
