#include "ofApp.h"
float radius;
float bg, fg;
ofPoint pos;
const int numPiano = 8;
const int numDrum = 8;
const int numGuitar = 6;
ofSoundPlayer  piano[numPiano];
ofSoundPlayer  drum[numDrum];
ofSoundPlayer guitar[numGuitar];
float volPiano[numPiano];
float volDrum[numDrum];
float volGuitar[numGuitar];
//--------------------------------------------------------------
void ofApp::setup(){
    mGui.setup("oscillator settings");
    mFreqSlider.setup("frequency", 200.0f, 60.0f, 2500.0f);
    mVolumeSlider.setup("volume(amplitude)", 0.2f,0.0f,0.89f);
    
    //ampersand means we pass in the variable location in memory
    //because inside the add function we create a pointer
    mGui.add(&mFreqSlider);
    mGui.add(&mVolumeSlider);
    // 2 output channels (stereo), 0 input channels
    // 512 samples per buffer, 2 buffers
    ofSoundStreamSetup(2, 0, sampleRate, 512, 2);
    
    // Wave
    ofSetVerticalSync(true);
    wavePhase = 0;
    pulsePhase = 0;
    
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    soundStream.setup(settings);
    
    //Circle
    ofBackground(0,0,0); //background color
    ofEnableAlphaBlending(); //opacity
    ofSetCircleResolution(120); //circlr resolution
    ofSetFrameRate(10); //frame rate per second
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(false);
    nBandsToGet = 1024;
    
    for ( int i=0; i<numPiano; i++) {
        piano[i].load(
                      "piano" + ofToString( i + 1 ) + ".mp3" );
        piano[i].setLoop( true );
        //Do some stereo panoraming of the sounds
        piano[i].setPan( ofMap( i, 0, numPiano-1, -0.5, 0.5 ) );
        
        piano[i].setVolume( 0 );
        piano[i].play();        //Start a sample to play
    }
    for ( int i=0; i<numDrum; i++) {
        drum[i].load(
                     "drum" + ofToString( i + 1 ) + ".mp3" );
        drum[i].setLoop( true );
        //Do some stereo panoraming of the sounds
        drum[i].setPan( ofMap( i, 0, numDrum-1, -0.5, 0.5 ) );
        
        drum[i].setVolume( 0 );
        drum[i].play();        //Start a sample to play
    }
    for ( int i=0; i<numGuitar; i++) {
        guitar[i].load(
                       "guitar" + ofToString( i + 1 ) + ".mp3" );
        guitar[i].setLoop( true );
        //Do some stereo panoraming of the sounds
        guitar[i].setPan( ofMap( i, 0, numGuitar-1, -0.5, 0.5 ) );
        
        guitar[i].setVolume( 0 );
        guitar[i].play();        //Start a sample to play
    }
    //(audio clipping)
    ofSoundSetVolume( 0.2 );
    
}
//--------------------------------------------------------------
void ofApp::update(){
    //Wave
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())
    unique_lock<mutex> lock(audioMutex);
    
    // this loop is building up a polyline representing the audio contained in
    // the left channel of the buffer
    
    // the x coordinates are evenly spaced on a grid from 0 to the window's width
    // the y coordinates map each audio sample's range (-1 to 1) to the height
    // of the window
    
    waveform.clear();
    for(size_t i = 0; i < lastBuffer.getNumFrames(); i++) {
        float sample = lastBuffer.getSample(i, 0);
        float x = ofMap(i, 0, lastBuffer.getNumFrames(), 0, ofGetWidth());
        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
        waveform.addVertex(x, y);
    }
    
    
    rms = lastBuffer.getRMSAmplitude();
    
    
    //    //Circle
   // radius = sin(2.0 * (float)mVolumeSlider) * 250.0f; //Change size with amplitude
    radius = sin(2.0 * (float)mVolumeSlider) * 250.0f;
    //bg = sin (ofGetElapsedTimef() * 2.0) * 127 + 127;
    fg = sin (ofGetElapsedTimef() * 3.0) * 127 + 127;
    pos.x = ofRandom(ofGetWidth());
    pos.y = ofRandom(ofGetHeight());
    
    
    float time = ofGetElapsedTimef();    //Get current time
    
    //Update volumes
    float tx = time*0.1 + 50;     //Value, smoothly changed over time
    for (int i=0; i<numGuitar; i++) {
        //Calculate the sample volume as 2D Perlin noise,
        //depending on tx and ty = i * 0.2
        float ty = i * 0.2;
        volGuitar[i] = ofNoise( tx, ty );    //Perlin noise
        
        guitar[i].setVolume( volGuitar[i] );    //Set sample's volume
    }
    for (int i=0; i<numPiano; i++) {
        //Calculate the sample volume as 2D Perlin noise,
        //depending on tx and ty = i * 0.2
        float ty = i * 0.2;
        volPiano[i] = ofNoise( tx, ty );    //Perlin noise
        
        piano[i].setVolume( volPiano[i] );    //Set sample's volume
    }
    /*for (int i=0; i<numDrum; i++) {
     //Calculate the sample volume as 2D Perlin noise,
     //depending on tx and ty = i * 0.2
     float ty = i * 0.2;
     volDrum[i] = ofNoise( tx, ty );    //Perlin noise
     
     drum[i].setVolume( volDrum[i] );    //Set sample's volume
     }*/
    ofSoundUpdate();
       fft = ofSoundGetSpectrum(nBandsToGet);
}
//--------------------------------------------------------------
void ofApp::draw(){
    //Circle
    ofBackground(bg, bg, bg); //set color for background
    ofSetColor(fg, fg, fg); // set color for circle
    

    
    //red and blue circles connect with oscillator
    if (ofDist(ofGetWidth()/2, ofGetHeight()/2, pos.x, pos.y)<200){
        ofSetColor(255, 63, 63, 127);
    } else {
        ofSetColor(63, 63, 255, 127);
    }
    ofCircle(pos.x, pos.y, radius);
  
    //Size depends on sound of guitar and piano. Position depends on Piano volume??
    for (int i=0; i<numPiano; i++) {
        int fx = ofMap(piano[i].getVolume(), 0.0, 1.0, 0.0, ofGetWidth());
        int fy = ofMap(piano[i].getVolume(), 0.0, 1.0, 0.0, ofGetHeight());
        int radius = ofMap(piano[i].getVolume(), 0.0, 1.0, 0.0, 100.0);
        //sound detection
        for (int i = 0;i < nBandsToGet; i++){
            //       ofSetColor(255,255,255, 127);
           // ofCircle(fx, fy, fft[i]*300);
            ofCircle(fx, fy, fft[i]*300);
        }
    }
    
    
    //Wave
    ofSetLineWidth(1 + (rms * 10.));
    waveform.draw();
    for (int i=0; i < ofGetHeight(); i+= 20){
        ofTranslate(0, i);
        waveform.draw();
        ofTranslate(0, -i);
        waveform.draw();
    }
    //Draw gui
    mGui.draw();
}
    
void ofApp::audioOut(ofSoundBuffer &outBuffer){
        
    for(int i = 0; i < outBuffer.size(); i += 2) {
        float sample = sin(mPhase*TWO_PI) * (float)mVolumeSlider; // generating a sine wave sample
            outBuffer[i] = sample; // writing to the left channel
            outBuffer[i+1] = sample; // writing to the right channel
            //memorize this equation! phaseOffset = freq / sampleRate
        float phaseOffset = ((float)mFreqSlider / (float)sampleRate);
            mPhase += phaseOffset;
            
        }
    //Wave
    // base frequency of the lowest sine wave in cycles per second (hertz)
    float frequency = 172.5;
    
    // mapping frequencies from Hz into full oscillations of sin() (two pi)
    float wavePhaseStep = (frequency / outBuffer.getSampleRate()) * TWO_PI;
    float pulsePhaseStep = (0.5 / outBuffer.getSampleRate()) * TWO_PI;
    
    // this loop builds a buffer of audio containing 3 sine waves at different
    // frequencies, and pulses the volume of each sine wave individually. In
    // other words, 3 oscillators and 3 LFOs.
    
    //    for(size_t i = 0; i < outBuffer.getNumFrames(); i++) {
    //
    //        // build up a chord out of sine waves at 3 different frequencies
    //        float sampleLow = sin(wavePhase);
    //        float sampleMid = sin(wavePhase * 1.5);
    //        float sampleHi = sin(wavePhase * 2.0);
    //
    //        // pulse each sample's volume
    //        sampleLow *= sin(pulsePhase);
    //        sampleMid *= sin(pulsePhase * 1.04);
    //        sampleHi *= sin(pulsePhase * 1.09);
    //
    //        float fullSample = (sampleLow + sampleMid + sampleHi);
    //
    //        // reduce the full sample's volume so it doesn't exceed 1
    //        fullSample *= 0.3;
    //
    //        // write the computed sample to the left and right channels
    //        outBuffer.getSample(i, 0) = fullSample;
    //        outBuffer.getSample(i, 1) = fullSample;
    //
    //        // get the two phase variables ready for the next sample
    //        wavePhase += wavePhaseStep;
    //        pulsePhase += pulsePhaseStep;
    //    }
    //
    unique_lock<mutex> lock(audioMutex);
    lastBuffer = outBuffer;
    

    
}
    void ofApp::mousePressed(int x, int y, int button){
    float widthStep = ofGetWidth() ;
    for (int i=0; i<numGuitar; i++) {
        //Calculate the sample volume as 2D Perlin noise,
        //depending on tx and ty = i * 0.2
        guitar[i].setPan(ofMap(x, 0, widthStep, -1, 1, true));
        guitar[i].setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);;    //Set sample's volume
    }
    
}
    void ofApp:: keyPressed(int key) {
    switch (key) {
        case('q'):
            for (int i=0; i<numGuitar; i++) {
                if (guitar[i].isPlaying()) {
                    guitar[i].stop();
                }
                else {
                    guitar[i].play();
                }
            }
            break;
        case('w'):
            for (int i=0; i<numPiano; i++) {
                if (piano[i].isPlaying()) {
                    piano[i].stop();
                }
                else {
                    piano[i].play();
                }
            }
            break;
            
    }
    
}
    void ofApp::mouseMoved(int x, int y ){
    

}
