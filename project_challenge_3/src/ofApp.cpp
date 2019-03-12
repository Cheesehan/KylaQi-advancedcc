#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(ofGetWidth(), ofGetHeight());
    camHeight = ofGetHeight();
    camWidth = ofGetWidth();
    ofSetVerticalSync(true);

    
    
    vector<ofVideoDevice> devices = cam.listDevices();
    
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(30);
    cam.initGrabber(camWidth, camHeight);
    
    threshold = 90.0;
    string sources[7] = {"do.mp3", "re.mp3", "mi.mp3", "fa.mp3", "sol.mp3", "la.mp3","si.mp3", };
    for (int i = 0; i < 7; i++) {
        MusicButton button = MusicButton();
        ofColor _c = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        button.setup(160*(1+i), 80, _c, "aud/"+sources[i]);
        buttons.push_back(button);
    }
    
    testCollage = Collage(bgPath, pathes, posVec, sizeVec);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (state == 0) {
        ofApp::updateState1();
    } else if (state == 1) {
        ofApp::updateState2();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if (state == 0) {
        ofApp::drawState1();
    } else if (state == 1) {
        ofApp::drawState2();
    }
}

//--------------------------------------------------------------
void ofApp::checkPiece(ofVec2f cursor){
    
    for(auto &p : testCollage.pieces){
        
        if( cursor.x > p.pos.x && cursor.x < p.pos.x+p.size.x && cursor.y > p.pos.y && cursor.y < p.pos.y+p.size.y){
            // cursor is winthin the range of this piece
            touching = true;
            
            if ( !holding ){
                // if we're not currently holding a piece
                p.counter += 1;
                p.isTouched = true;
               
            }
        } else {
            p.isTouched = false;
            p.counter = 0;
            //touching = false;
        }
        
    }
    
    
}

void ofApp::updateState1() {
    vec.clear();
    count = 0;
    avgX = 0;
    avgY = 0;
    points = 0;
    previous_pixels = cam.getPixels();
    previous_pixels.mirror(false, true);
    cam.update();
    video_pixels = cam.getPixels();
    video_pixels.mirror(false, true);
    
    for (int y = 0; y < camHeight; y++) {
        for (int x = 0; x < camWidth; x++) {
            int index = x + y * camWidth;
            int red_index = index * 3;
            int green_index = index * 3 + 1;
            int blue_index = index * 3 + 2;
            
            if (ofDist(video_pixels[red_index], video_pixels[green_index], video_pixels[blue_index],
                       previous_pixels[red_index], previous_pixels[green_index], previous_pixels[blue_index]) > threshold) {
                vec.push_back(ofPoint(x, y));
                count ++;
                
            }
        }
    }
    
    for (auto &b : buttons) {
        b.update(vec);
    }
}

void ofApp::updateState2() {
    int counter = 0;
    if (cam.isFrameNew()) {
        video_pixels = cam.getPixels();
        video_pixels.mirror(false, true);
        
        for (int x = 0; x < camWidth; x++) {
            for (int y = 0; y < camHeight; y++) {
                ofColor temp = video_pixels.getColor(x, y);
                if(temp.r >= 250 && temp.g >= 250 && temp.b >= 250 ) {
                    midX += x;
                    midY += y;
                    counter += 1;
                }
            }
        }
        
        if (counter != 0) {
            midX /= counter;
            midY /= counter;
        } else {
            midX = -1;
            midY = -1;
        }
        
    }
    cam.update();
    
    // check if any piece is touched
    checkPiece(ofVec2f(midX, midY));
    // check piec
    testCollage.update(ofVec2f(midX, midY));
}

void ofApp::drawState1() {
    if (count > 200 && count < 20000) {
        for (auto &v: vec) {
            ofDrawCircle(v, 3);
        }
    }
    
    for (int i = 0; i < buttons.size() ;i++) {
        buttons[i].draw();
    }
}

void ofApp::drawState2() {
        testCollage.draw();
    
        ofDrawEllipse(midX, midY, 50, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {
        state = 1;
    }
    
//    // isSelected
//    if(key == ' ' && touching){
//        // if spacebar is down & we're currently touching a piece
//        for(auto &p : testCollage.pieces){
//            // set this piece's isSelected value to its isTouched value
//            p.isSelected = p.isTouched;
//
//        }
//        holding = true;
//
//    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
//    for(auto &p : testCollage.pieces){
//
//        ofLog() << p.isSelected;
//
//    }
//
//    if(key == ' '){
//        for(auto &p : testCollage.pieces){
//
//            p.isSelected = false;
//
//        }
//    }
//
//    holding = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
