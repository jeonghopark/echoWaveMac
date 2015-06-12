#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();

    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);

    
    ofMesh mesh;
    
    ofEasyCam cam;
    
    vector<float> zPos;
    vector<float> zDirection;
    
    int numWidth, numHeight;
    float plateWidth, plateHeight;
    
    int vertexSpacing;
    
    float zSize;
    
    // Audio Beispiele kommt von "audioInputExample"
    int bufferSize;
//    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> inputBuffer;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    //    ofSoundStream soundStream;
    int	initialBufferSize;
    int	sampleRate;
    float * buffer;
    int volNumHeight;
    
    

};


