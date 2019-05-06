#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    receiver.setup(8338);
    dub.load ("dub.wav");
    dub.play();
    dub.setLoop(true);
    vidGrabber.setup(320,240);
    vidGrabber.setVerbose(true);
    vol = ofMap(posX,0,ofGetWidth(),0.5,1);
    ofSetFrameRate(30);
    glm::ivec2 size = glm::ivec2(vidGrabber.getWidth(), vidGrabber.getHeight());
    
    colorImg.allocate(size.x,size.y);
    grayImage.allocate(size.x,size.y);
    grayBg.allocate(size.x,size.y);
    grayDiff.allocate(size.x,size.y);
    
    bLearnBakground = true;
    threshold = 120;
    ofSetBackgroundAuto(false);
    
    for(int i = 0; i < number_of_colors; i++) {
        bg = ofColor(203,143,144);
        if (i == 0)
            color_set[i] = ofColor(252,252,75 );
        else {
            color_set[i] = color_set[i-1].lerp(bg, .3);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
       dub.setVolume(vol);
    //ofBackground(100,100,100);
    
    // bool bNewFrame = false;
    vidGrabber.update();
    
    for (int x = 0; x < 52; x++) {
        for (int y = 0; y < 39; y++) {
            colors[x][y] = bg;
        }
    }
    
    //check if the frame is new
    // bNewFrame = vidGrabber.isFrameNew();
    bool bNewFrame = true;
    
    if (bNewFrame){
        
        
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg;
        
        if (bLearnBakground == true){
            grayBg = grayImage;        // the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }
        
        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 150, (340*240)/3, 10, true);    // find holes
    }
    
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if(m.getAddress() == "/pose/position"){
            //            ofLog() << m.getArgAsFloat(0) << " "<< m.getArgAsFloat(1) << endl;
            posX = m.getArgAsFloat(0); //640
            posY = m.getArgAsFloat(1); //480
            
            posX = ofMap(posX,0,640, ofGetWidth(), 0);
            posY = ofMap(posY,0,480,0, ofGetHeight());
        }
    }
    vol = ofMap(posX,0,ofGetWidth(),0.5,1);

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    points.clear();
    
    // draw the incoming video image
    ofSetHexColor(0xffffff);
    // ofPushMatrix();
    // ofTranslate(ofGetWidth()/2 - colorImg.getWidth()/2,ofGetHeight()/2 - colorImg.getHeight()/2);
    grayDiff.draw(0,0);
    
    //if we want to draw an outline around our blob path
    ofNoFill();
    ofSetColor(ofColor::orange);
    
    ofBeginShape();
    //we loop through each of the detected blobs
    //contourFinder.nBlobs gives us the number of detected blobs
    for (int i = 0; i < contourFinder.nBlobs; i++){
        //each of our blobs contains a vector<ofPoints> pts
        for(int j=0; j < contourFinder.blobs[i].pts.size(); j++){
            ofVertex(contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y);
            
        }
        
        ofRectangle box = contourFinder.blobs[i].boundingRect;
        
        ofNoFill();
        ofSetColor(255);
        //        ofDrawRectangle(box.getX(), box.getY(), box.getWidth(), box.getHeight());
        
        points.push_back(box.getCenter());
    }
    for(auto point : points) {
        int x = point.x / 20;
        int y = point.y / 20;
        for (int delX = 0; delX < number_of_colors; delX++) {
            for (int delY = 0; delY < number_of_colors; delY++) {
                if (x + delX >= 0 && x + delX < 52 && y + delY >= 0 && y + delY < 39 && x - delX >= 0 && x - delX < 52 && y - delY >= 0 && y - delY < 39) {
                    colors[x + delX][y + delY] = color_set[delX];
                    colors[x - delX][y + delY] = color_set[delX];
                    colors[x + delX][y - delY] = color_set[delX];
                    colors[x - delX][y - delY] = color_set[delX];

                }
            }
        }
    }
    
    
   
    ofFill();
    ofBackground(255);
    
    for (int x = 0; x < ofGetWidth(); x+=20){
        for(int y = 0; y < ofGetHeight(); y+=20){
            
            float n= ofNoise(x* 0.005, y* 0.005, ofGetElapsedTimeMillis() * 0.0008);
            ofPushMatrix();
            ofPushStyle();
            ofFill();
            ofSetColor(colors[x/20][y/20]);
            ofTranslate(x, y);
            ofRotateDeg(TWO_PI *n );
            ofScale(20*n);
            ofDrawRectangle(0, 0, 1, 1);
            ofPopStyle();
            ofPopMatrix();
            
            
            
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            
            
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
        case ' ':
            bLearnBakground = true;
            break;
      
//        case '1':
//            curr = ofColor(55,64,136);
//            tar = ofColor(0,0,0);
//            bg = ofColor(0,0,0);
//            break;
//
//        case '2':
//            curr = ofColor(122,109,215);
//            tar = ofColor(0,0,0);
//            bg = ofColor(0,0,0);
//            break;
//
//        case '3':
//            curr = ofColor(115,159,123);
//            tar = ofColor(0,0,0);
//            bg = ofColor(0,0,0);
//            break;
            
        case '1':
            for(int i = 0; i < number_of_colors; i++) {
                bg = ofColor(213,184,175);
                if (i == 0)
                color_set[i] = ofColor(108,104,177);
                else {
                    color_set[i] = color_set[i-1].lerp(bg, .3);
                }
            }
            
            break;
            
        case '2':
            for(int i = 0; i < number_of_colors; i++) {
                bg = ofColor(239,195,94);
                if (i == 0)
                    color_set[i] = ofColor(210,107,107);
                else {
                    color_set[i] = color_set[i-1].lerp(bg, .3);
                }
            }
            
            break;
            
        case '3':
            for(int i = 0; i < number_of_colors; i++) {
                bg = ofColor(214,235,254);
                if (i == 0)
                    color_set[i] = ofColor(29,25,248);
                else {
                    color_set[i] = color_set[i-1].lerp(bg, .3);
                }
            }
            
            break;
            
        case '4':
            for(int i = 0; i < number_of_colors; i++) {
                bg = ofColor(203,143,144);
                if (i == 0)
                    color_set[i] = ofColor(252,252,75 );
                else {
                    color_set[i] = color_set[i-1].lerp(bg, .3);
                }
            }
            
            break;
            
        case '5':
            for(int i = 0; i < number_of_colors; i++) {
                bg = ofColor(131,86,153);
                if (i == 0)
                    color_set[i] = ofColor(238,190,192);
                else {
                    color_set[i] = color_set[i-1].lerp(bg, .3);
                }
            }
            
            break;
            
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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
