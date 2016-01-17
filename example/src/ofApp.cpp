#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // setup spreadsheet (x, y, number of rows to display, number of cols to display)
    spreadsheet.setup(20, 80, 30, 8);

    
    // specify column headers
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
    
    spreadsheet.setHeaders(headers);
    
    // add entries
    
    for (int i=0; i<50; i++) {
        vector<string> entry;
        entry.push_back(ofToString(ofRandom(1)));
        entry.push_back(ofToString(ofRandom(-50, -30)));
        entry.push_back(ofToString(ofRandom(100, 180)));
        entry.push_back(ofToString(ofRandom(20, 50)));
        entry.push_back(ofToString(ofRandom(35, 90)));
        entry.push_back(ofToString(ofRandom(-100, 100)));
        entry.push_back(ofToString(ofRandom(-30, 20)));
        entry.push_back(ofToString(ofRandom(1)));
        entry.push_back(ofToString(ofRandom(-50, -30)));
        entry.push_back(ofToString(ofRandom(100, 180)));
        entry.push_back(ofToString(ofRandom(20, 50)));
        entry.push_back(ofToString(ofRandom(35, 90)));
        entry.push_back(ofToString(ofRandom(-100, 100)));
        entry.push_back(ofToString(ofRandom(-30, 20)));
        entry.push_back(ofToString(ofRandom(35, 90)));
        entry.push_back(ofToString(ofRandom(-100, 100)));
        entry.push_back(ofToString(ofRandom(-30, 20)));
        
        spreadsheet.addEntry(entry);
    }
    
    // you can get the entries and headers from the spreadsheet
    vector<vector<string> > myEntries = spreadsheet.getEntries();
    vector<string> myHeaders = spreadsheet.getHeaders();
    reverse = false;
}
//--------------------------------------------------------------
void ofApp::update()
{
    
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0, 100, 0);
    ofDrawBitmapString("click on spreadsheet to select an entry.\nup/down/left/right to scroll through spreadsheet (hold shift to scroll by entire pages).\nbackspace to delete an entry.\nTo Sort a Column, Left Click for Acending Order and Right Click for Decending Order", 20, 20);
    
    spreadsheet.draw();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    int headersSize = spreadsheet.getHeaders().size();
    for (int i = 0; i < headersSize; i++) {
        if (key == ofToChar(ofToString(i))) {
            spreadsheet.sortByCol(i,reverse);
        }
    }
    if (key == 'r') {
        reverse = !reverse;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}