#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // setup spreadsheet (x, y, number of rows to display, number of cols to display)
    spreadsheet.setup(20, 80, 30, 8);
    
    csv.loadFile(ofToDataPath("data.csv"));
    // specify column headers
    vector<string> headers;
    int headerNumber = csv.data[0].size();
    for (int i = 0; i < headerNumber; i++) {
        headers.push_back(ofToString(csv.data[0][i]));
    }
    
    
    spreadsheet.setHeaders(headers);
    int entry = csv.data.size();

    // Ignore the Header Row
    for (int i = 1; i < entry; i++) {
        details.push_back(myStruct(ofToInt(csv.data[i][0]), ofToInt(csv.data[i][1]), ofToString(csv.data[i][2]), ofToString(csv.data[i][3]),ofToFloat(csv.data[i][4])));
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
    if (key == 's') {
        vector<vector<string> > data;
        data.push_back(spreadsheet.getHeaders());
        
        int entryNumbers = spreadsheet.getEntries().size();
        for (int i = 0; i < entryNumbers; i++) {
            data.push_back(spreadsheet.getEntries()[i]);
        }

        csv.setData(data);
        csv.saveFile();
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