#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    vector<string> headers;
    headers.push_back("col1");
    headers.push_back("col2");
    headers.push_back("col3");
    headers.push_back("col4");
    headers.push_back("col5");
    headers.push_back("col6");
    headers.push_back("col7");
    headers.push_back("col8");
    headers.push_back("col9");
    headers.push_back("col10");
    headers.push_back("col11");
    headers.push_back("col12");
    headers.push_back("col13");
    headers.push_back("col14");
    headers.push_back("col15");
    headers.push_back("col16");
    headers.push_back("col17");
    
    spreadsheet.setup(500, 500);
    spreadsheet.setHeaders(headers);
    
    for (int i=0; i<50; i++) {
        vector<float> entry;
        entry.push_back(ofRandom(1));
        entry.push_back(ofRandom(-50, -30));
        entry.push_back(ofRandom(100, 180));
        entry.push_back(ofRandom(20, 50));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        entry.push_back(ofRandom(1));
        entry.push_back(ofRandom(-50, -30));
        entry.push_back(ofRandom(100, 180));
        entry.push_back(ofRandom(20, 50));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        spreadsheet.addEntry(entry);
    }
}


//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0, 255, 0);
    spreadsheet.draw(20, 20);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') {
        vector<float> entry;
        entry.push_back(ofRandom(1));
        entry.push_back(ofRandom(-50, -30));
        entry.push_back(ofRandom(100, 180));
        entry.push_back(ofRandom(20, 50));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        entry.push_back(ofRandom(1));
        entry.push_back(ofRandom(-50, -30));
        entry.push_back(ofRandom(100, 180));
        entry.push_back(ofRandom(20, 50));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        entry.push_back(ofRandom(35, 90));
        entry.push_back(ofRandom(-100, 100));
        entry.push_back(ofRandom(-30, 20));
        spreadsheet.addEntry(entry);
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
