//
//  Piece.hpp
//  project_challenge_3
//
//  Created by Casey Pan on 3/8/19.
//

#ifndef Piece_hpp
#define Piece_hpp

#include <stdio.h>
#include "ofMain.h"

class Piece{
    
public:
    
    string FILE_PATH;
    ofImage graph;
    ofVec2f pos;
    ofVec2f size;
    
    int counter = 0;
    
    bool isTouched=false;
    bool isSelected=false;
    
    Piece();
    Piece(string path, ofVec2f posVec, ofVec2f sizeVec);
    
    void draw();
    
    void update(ofVec2f cursor);
    
    void move(ofVec2f cursor);
};

#endif /* Piece_hpp */
