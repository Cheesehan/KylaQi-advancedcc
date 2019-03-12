//
//  Piece.cpp
//  project_challenge_3
//
//  Created by Casey Pan on 3/8/19.
//

#include "Piece.hpp"

Piece::Piece(){
    ofLogNotice() << "a Piece is constructed" << endl;
}

Piece::Piece(string path, ofVec2f posVec, ofVec2f sizeVec){
    
    FILE_PATH = path;
    pos = posVec;
    size = sizeVec;
    isSelected = false;
    
    graph.load(FILE_PATH);
}


void Piece::move(ofVec2f cursor){
    
    pos.x = cursor.x-size.x/2;
    pos.y = cursor.y-size.y/2;
    
}

void Piece::update(ofVec2f cursor) {
    
    isSelected = counter >= 10;
    
    if (isSelected) {
        Piece::move(cursor);
    }
}


void Piece::draw(){
    graph.draw(pos.x, pos.y, size.x, size.y);
}
