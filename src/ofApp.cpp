#include "ofApp.h"
#include <Poco/Path.h>

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofBackground(10, 255);
    ofSetDepthTest(true);
    
    inputBuffer.clear();
    volHistory.clear();
    volNumHeight = 256;
    
    bufferSize = 256;
    initialBufferSize = 512;
    sampleRate = 44100;
    
    inputBuffer.assign(bufferSize, 0.0);
    volHistory.assign(volNumHeight, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    buffer = new float[initialBufferSize];
    memset(buffer, 0, initialBufferSize * sizeof(float));
    volHistory.push_back( scaledVol );
    
    
    
    ofSoundStreamSetup(2, 1, this, sampleRate, initialBufferSize, 1);
    
    // ofxPD
    int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
    int numInputs = 1;
    core.setup(2, numInputs, sampleRate, ticksPerBuffer);
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    numWidth = 256 * 1;
    numHeight = 512 * 0.3;
    vertexSpacing = 7;
    
    plateWidth = (numWidth-1) * vertexSpacing;
    plateHeight = (numHeight-1) * vertexSpacing;
    
    zSize = 60;
    
    float _xRandom = ofRandom(24,48);
    float _yRandom = ofRandom(24,48);
    
    float _movingSpeed = 2.5;
    for (int j=0; j<numHeight; j++) {
        for (int i=0; i<numWidth; i++) {
            ofVec3f _a = ofVec3f( i * vertexSpacing - plateWidth/2, j * vertexSpacing - plateHeight/2, 0 );
            mesh.addVertex(_a);
            ofColor _c = ofColor::fromHsb(0, 120, 255, 255);
            mesh.addColor( _c );
            
            
            float _noise = ofNoise( i/_xRandom, j/_yRandom ) * zSize;
            
            if (_noise>zSize/2) {
                zPos.push_back( zSize - _noise );
                zDirection.push_back( -_movingSpeed );
            } else {
                zPos.push_back( _noise );
                zDirection.push_back( _movingSpeed );
            }
        }
    }
    
    for (int j=0; j<numHeight-1; j++) {
        for (int i=0; i<numWidth-1; i++) {
            int _index = i + j * numWidth;
            
            mesh.addIndex(_index);
            mesh.addIndex(_index+numWidth);
            mesh.addIndex(_index+1);
            
            mesh.addIndex(_index+1);
            mesh.addIndex(_index+numWidth);
            mesh.addIndex(_index+numWidth+1);
        }
    }
    
    cam.setupPerspective();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    core.update();
    
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 15.0, true);
    volHistory.push_back( scaledVol );
	   
    if( volHistory.size() > numWidth ){
        volHistory.erase(volHistory.begin());
    }
    
    if (volHistory.size()>0) {
        for (int j=0; j<numHeight; j++) {
            for (int i=0; i<numWidth; i++) {
                int _index = i + j * numWidth;
                
                zPos[_index] = zPos[_index] + zDirection[_index];
                
                if (zPos[_index]>zSize/2)   zDirection[_index] = -zDirection[_index];
                if (zPos[_index]<0)         zDirection[_index] = -zDirection[_index];
                
            }
        }
        
        for (int j=0; j<numHeight; j++) {
            for (int i=0; i<numWidth; i++) {
                int _index = i + j * numWidth;
                int _volIndex = _index % numWidth;
                
                ofVec3f _vec = mesh.getVertex(_index);
                mesh.setVertex( _index, ofVec3f( _vec.x, _vec.y, zPos[_index] * volHistory.at(numWidth-_volIndex-1) ));
                
            }
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //    core.draw();
    
    cam.begin();
    
    ofRotateX(82);
    ofRotateZ(90);
    
    //    for (int j=0; j<numHeight; j++) {
    //        for (int i=0; i<numWidth; i++) {
    //            int _index = i + j * numWidth;
    //            mesh.setColor( _index, ofColor::fromHsb(0,0,255,255) );
    //        }
    //    }
    //
    //    mesh.draw();
    //
    //    for (int j=0; j<numHeight; j++) {
    //        for (int i=0; i<numWidth; i++) {
    //            int _index = i + j * numWidth;
    //            mesh.setColor( _index, ofColor::fromHsb(0,0,0,255) );
    //        }
    //    }
    //
    //    mesh.drawWireframe();
    

    for (int j=0; j<numHeight; j+=3) {
        for (int i=0; i<numWidth; i+=2) {
            int _index = i + j * numWidth;
            ofPushMatrix();
            ofPushStyle();
            ofVec3f _vecVertex = mesh.getVertex(_index);
            ofTranslate(_vecVertex.x, _vecVertex.y, _vecVertex.z);
            
            if (_vecVertex.z<5) {
                _vecVertex.z = 0;
            }
            
            ofRotate(sin(_vecVertex.z)*360, 1, 1, 1);
            
//            ofNoFill();
            ofSetColor(ofColor::fromHsb(0, 0, 255, 180));
            
            ofRect( 0, 0, 0, 3, 3);
            
            ofPopStyle();
            ofPopMatrix();
        }
    }
    
    cam.end();
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(),1), 10, 10);
    
    //    core.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
    core.exit();
}


//void ofApp::audioIn(float * input, int bufferSize, int nChannels){

//    float curVol = 0.0;
//    int numCounted = 0;
//
//    for (int i = 0; i < bufferSize; i++){
//        inputBuffer[i]		= input[i];
//        curVol += inputBuffer[i] * inputBuffer[i];
//        numCounted+=2;
//    }
//
//    curVol /= (float)numCounted;
//    curVol = sqrt( curVol );
//
//    smoothedVol *= 0.93;
//    smoothedVol += 0.07 * curVol;
//
//    bufferCounter++;

//}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
    
    float curVol = 0.0;
    int numCounted = 0;
    
    for (int i = 0; i < bufferSize; i++){
        inputBuffer[i]		= input[i];
        curVol += inputBuffer[i] * inputBuffer[i];
        numCounted+=2;
    }
    
    curVol /= (float)numCounted;
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
    core.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
    core.audioRequested(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key=='f')
        ofToggleFullscreen();
    
    //    core.keyPressed(key);
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
