#include "ofxSpreadsheet.h"

//--------------------------------------------------------------
ofxSpreadsheet::ofxSpreadsheet()
{
    cellWidth = 100;
    cellHeight = 18;
}
//--------------------------------------------------------------
void ofxSpreadsheet::setup(int x, int y, int numDisplayRows, int numDisplayCols)
{
    
    this->numDisplayRows = numDisplayRows;
    this->numDisplayCols = numDisplayCols;
    width = numDisplayCols * cellWidth;
    height = (numDisplayRows + 1) * cellHeight;
    
    setPosition(x, y);
    setTopRow(0);
    setLeftCol(0);
    selectRow(0);
    setActive(true);
}
//--------------------------------------------------------------
void ofxSpreadsheet::sortByCol(int col,bool direction)
{
    if (direction) {
        sort(entries.begin(), entries.end(), [&col](const vector<string> &a, const vector<string> &b){
            if (std::isdigit(ofToChar(a[col])) && std::isdigit(ofToChar(b[col]))) {
                return ofToFloat(a[col]) > ofToFloat(b[col]);
            }
            else {
                return a[col] > b[col];
            }
        });
    }
    else {
        sort(entries.begin(), entries.end(), [&col](const vector<string> &a, const vector<string> &b){
            if (std::isdigit(ofToChar(a[col])) && std::isdigit(ofToChar(b[col]))) {
                return ofToFloat(a[col]) < ofToFloat(b[col]);
            }
            else {
                return a[col] < b[col];
            }
        });
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::setActive(bool active)
{
    this->active = active;
    if (active) {
        ofAddListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &ofxSpreadsheet::keyReleased);
        ofAddListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
        ofAddListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
    }
    else {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxSpreadsheet::keyReleased);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}
//--------------------------------------------------------------
void ofxSpreadsheet::setHeaders(vector<string> headers)
{
    this->headers = headers;
}
//--------------------------------------------------------------
void ofxSpreadsheet::addEntry(vector<string> entry)
{
    entries.push_back(entry);
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxSpreadsheet::deleteSelectedRow()
{
    if (entries.size() == 0) return;
    entries.erase(entries.begin() + min((int) entries.size()-1, selectedRow));
    selectRow(min(selectedRow, (int) entries.size()-1));
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxSpreadsheet::clearEntries()
{
    entries.clear();
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxSpreadsheet::draw()
{
    int numRows = min(numDisplayRows, (int) entries.size() - topRow);
    int numCols = min(numDisplayCols, (int) headers.size() - leftCol);
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
    
    // draw header
    for (int c = 0; c < numCols; c++) {
        float cx = c * cellWidth;
        ofFill();
        ofSetColor(55);
        ofDrawRectangle(cx, 0, cellWidth, cellHeight);
        ofSetColor(110);
        ofNoFill();
        ofDrawRectangle(cx, 0, cellWidth, cellHeight);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(headers[leftCol + c]), cx + 1, cellHeight - 1);
    }
    
    // draw cells
    ofTranslate(0, cellHeight);
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            float cy = r * cellHeight;
            float cx = c * cellWidth;
            ofFill();
            ofSetColor(topRow + r == selectedRow ? 210 : 255);
            ofDrawRectangle(cx, cy, cellWidth, cellHeight);
            ofSetColor(170);
            ofNoFill();
            ofDrawRectangle(cx, cy, cellWidth, cellHeight);
            ofSetColor(0);
            ofDrawBitmapString(ofToString(entries[topRow + r][leftCol + c]), cx + 1, cy + cellHeight - 1);
        }
    }
    
    ofPopMatrix();
    ofPopStyle();
}
//--------------------------------------------------------------
void ofxSpreadsheet::keyPressed(ofKeyEventArgs &evt)
{
    if (evt.key == OF_KEY_UP) {
        scrollUp();
    }
    else if (evt.key == OF_KEY_DOWN) {
        scrollDown();
    }
    else if (evt.key == OF_KEY_LEFT) {
        scrollLeft();
    }
    else if (evt.key == OF_KEY_RIGHT) {
        scrollRight();
    }
    else if (evt.key == OF_KEY_BACKSPACE) {
        deleteSelectedRow();
    }
    else if (evt.key == OF_KEY_SHIFT) {
        shift = true;
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::keyReleased(ofKeyEventArgs &evt)
{
    if (evt.key == OF_KEY_SHIFT) {
        shift = false;
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::mousePressed(ofMouseEventArgs &evt)
{
    if (ofRectangle(x, y, width, height).inside(evt.x, evt.y)) {
        int idxY = floor((evt.y - y - cellHeight) / cellHeight);
        int idyX = floor((evt.x - x - cellWidth) / cellWidth)+1;
        selectRow(topRow + idxY);
        
        // Left Click : Right Click
        sortByCol(leftCol+ idyX, evt.button);
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::mouseDragged(ofMouseEventArgs &evt)
{
    
}
//--------------------------------------------------------------
void ofxSpreadsheet::mouseReleased(ofMouseEventArgs &evt)
{
    
}
//--------------------------------------------------------------
void ofxSpreadsheet::scrollUp()
{
    selectRow(max(0, selectedRow - (shift ? numDisplayRows-1 : 1)));
    if (selectedRow < topRow) {
        setTopRow(selectedRow);
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::scrollDown()
{
    selectRow(min((int) entries.size()-1, selectedRow + (shift ? numDisplayRows-1: 1)));
    
    if (selectedRow >= topRow + numDisplayRows) {
        setTopRow(topRow + (shift ? numDisplayRows-1: 1));
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::scrollLeft()
{
    if (leftCol > 0) {
        setLeftCol(max(0, leftCol - (shift ? numDisplayCols-1 : 1)));
    }
}
//--------------------------------------------------------------
void ofxSpreadsheet::scrollRight()
{
    setLeftCol(min((int)headers.size() - numDisplayCols, leftCol + (shift ? numDisplayRows-1 : 1)));
}
