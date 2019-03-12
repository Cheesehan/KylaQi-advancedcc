//
//  Collage.hpp
//  project_challenge_3
//
//  Created by Casey Pan on 3/8/19.
//

#ifndef Collage_hpp
#define Collage_hpp

#include <stdio.h>
#include "Piece.hpp"
#include "ofMain.h"

class Collage{
    
public:
    vector<string> FILE_PATHES;
    ofImage bg;
    vector<ofImage> imgs;
    vector<ofVec2f> poses;
    vector<ofVec2f> sizes;
    
    vector<Piece> pieces;
    
    Collage();
    Collage(string bg_path, vector<string> pathes, vector<ofVec2f> posVec, vector<ofVec2f> sizeVec);
    //~Collage();
    
    void update(ofVec2f cursor);
    void draw();
    
};

#endif /* Collage_hpp */
