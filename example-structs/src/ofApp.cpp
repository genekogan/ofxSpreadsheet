#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // setup spreadsheet (x, y, number of rows to display, number of cols to display)
    spreadsheet.setup(20, 80, 30, 8);
    
    // specify column headers
    vector<string> headers;
    headers.push_back("Entry ID");
    headers.push_back("Customer ID");
    headers.push_back("First Name");
    headers.push_back("Surname Name");
    headers.push_back("Balance");
    
    spreadsheet.setHeaders(headers);
    
    // add entries
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    
    
    
    for (int i=0; i<50; i++) {
        int f = ofRandom(0, 26);
        int s = ofRandom(0, 26);
        details.push_back(myStruct(i,ofRandom(0, 200),ofToString(alphabet[f]),ofToString(alphabet[s]),ofRandom(-1000,100000)));
    }
    
    for (int i = 0; i < details.size(); i++) {
        spreadsheet.addEntry(details[i].getDetails());
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
void ofApp::mouseEntered(int x, int y)
{

}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
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