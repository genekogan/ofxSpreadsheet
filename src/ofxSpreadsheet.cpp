#include "ofxSpreadsheet.h"

//-------
void ofxSpreadsheetScrollable::load(ofFbo *text, float w, float h) {
    this->fbo = text;
    ofFbo::allocate(w, h, GL_RGBA32F_ARB);
    position = 0;
}

//-------
void ofxSpreadsheetScrollable::update() {
    ofPushStyle();
    ofFbo::begin();
    ofClear(0, 0);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofSetColor(255);
    fbo->draw(0, position);
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofPushMatrix();
    ofPopMatrix();
    ofPushMatrix();
    ofPopMatrix();
    ofFbo::end();
    ofPopStyle();
}

//-------
void ofxSpreadsheet::setup(int width, int height) {
    this->width = width;
    this->height = height;
    
    headerHeight = 20;
    scrollbarWidth = 20;
    cellHeight = 20;

    dragging = false;
    selectMultiple = false;

    fboHeader.allocate(width, headerHeight);
    fboScrollBar.allocate(scrollbarWidth, height-fboHeader.getHeight());
    fboSheet.allocate(width-fboScrollBar.getWidth(), height-fboHeader.getHeight());
    
    sheetWidth = width-fboScrollBar.getWidth();
    sheetHeight = height-fboHeader.getHeight();
    sheet.load(&fboSheet, sheetWidth, sheetHeight);

    drawSpreadsheet();
    drawScrollBar();
    
    ofAddListener(ofEvents().update, this, &ofxSpreadsheet::update);
    ofAddListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxSpreadsheet::keyReleased);
    ofAddListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
    ofAddListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
}

//-------
void ofxSpreadsheet::setHeaders(vector<string> headers) {
    this->headers = headers;
    cellWidth = fboSheet.getWidth() / headers.size();
    
    fboHeader.begin();
    ofBackground(0);
    for (int i=0; i<headers.size(); i++) {
        drawCell(0, i, headers[i], HEADER);
    }
    fboHeader.end();
}

//-------
void ofxSpreadsheet::addEntry(vector<float> entry) {
    entries.push_back(entry);
    fboSheet.begin();
    for (int j=0; j<entry.size(); j++) {
        drawCell(entries.size()-1, j, ofToString(entry[j]));
    }
    fboSheet.end();
    cellsHeight = entries.size() * cellHeight;
    if (entries.size() == 1) {
        selection.push_back(0);
        highlightRows(selection, true);
    }
}

//-------
void ofxSpreadsheet::update(ofEventArgs &data) {
    sheet.update();
    drawScrollBar();
}

//-------
void ofxSpreadsheet::highlightRows(vector<int> rows, bool highlight) {
    CellType type = highlight ? SELECTED : CELL;

    fboSheet.begin();
    for (int r=0; r<rows.size(); r++) {
        vector<float> row = entries[rows[r]];
        for (int i=0; i<row.size(); i++) {
            drawCell(rows[r], i, ofToString(row[i]), type);
        }
    }
    fboSheet.end();
}


//-------
void ofxSpreadsheet::selectRow(int row) {
    highlightRows(selection, false);
    
    int minSelect = entries.size();
    int maxSelect = 0;
    if (selection.size() == 0) {
        minSelect = 0;
        maxSelect = row;
    }
    else {
        for (int i=0; i<selection.size(); i++) {
            if      (selection[i] > maxSelect)  maxSelect = selection[i];
            if (selection[i] < minSelect)  minSelect = selection[i];
        }
        if      (row > maxSelect)   maxSelect = row;
        else if (row < minSelect)   minSelect = row;
    }
    
    selection.clear();
    if (selectMultiple) {
        for (int r=minSelect; r<=maxSelect; r++) {
            selection.push_back(r);
        }
    }
    else {
        selection.push_back(row);
    }
    highlightRows(selection, true);
}

//-------
void ofxSpreadsheet::deleteSelectedRows() {
    entries.erase(entries.begin() + selection[0], entries.begin() + selection[selection.size()-1] + 1);

    drawSpreadsheet();
    cellsHeight = entries.size() * cellHeight;
    scrollHeight = (height - fboHeader.getHeight()) * fboScrollBar.getHeight() / cellsHeight;
    scrollTop = ofClamp(scrollTop, 0, abs(height - scrollHeight - fboHeader.getHeight()));
    sheet.setPosition(-scrollTop * cellsHeight / fboScrollBar.getHeight());

    int r = min((int) selection[0], (int) entries.size()-1);
    selection.clear();
    selection.push_back(r);
    
    highlightRows(selection, true);
}

//-------
void ofxSpreadsheet::keyPressed(ofKeyEventArgs &evt) {
    if (evt.key == OF_KEY_UP) {
        selectRow(ofClamp(selection[0]-1, 0, entries.size()-1));
    }
    else if (evt.key == OF_KEY_DOWN) {
        selectRow(ofClamp(selection[selection.size()-1]+1, 0, entries.size()-1));
    }
    else if (evt.key == OF_KEY_BACKSPACE) {
        deleteSelectedRows();
    }
    else if (evt.key == OF_KEY_SHIFT) {
        selectMultiple = true;
    }
}

//-------
void ofxSpreadsheet::keyReleased(ofKeyEventArgs &evt) {
    if (evt.key == OF_KEY_SHIFT) {
        selectMultiple = false;
    }
}

//-------
void ofxSpreadsheet::mousePressed(ofMouseEventArgs &evt){
    ofRectangle sheetRect(drawPosition.x, drawPosition.y,
                          sheetWidth, sheetHeight);
    if (sheetRect.inside(evt.x, evt.y)){
        float position = evt.y - drawPosition.y - sheet.getPosition();
        int row = floor(position / cellHeight);
        if (row < entries.size())
            selectRow(row);
    }
    else {
        ofRectangle bar(drawPosition.x + fboSheet.getWidth(),
                        drawPosition.y + scrollTop,
                        fboScrollBar.getWidth(), scrollHeight);

        if (bar.inside(evt.x, evt.y)) {
            mouseHold.set(evt.x, evt.y);
            dragging = true;
        }
    }
}

//-------
void ofxSpreadsheet::mouseDragged(ofMouseEventArgs &evt){
    if (dragging) {
        scrollTop = ofClamp(scrollTop + (evt.y - mouseHold.y), 0, height - scrollHeight - fboHeader.getHeight());
        sheet.setPosition(-scrollTop * cellsHeight / fboScrollBar.getHeight());
        mouseHold.set(mouseHold.x, evt.y);
    }
}

//-------
void ofxSpreadsheet::mouseReleased(ofMouseEventArgs &evt){
    dragging = false;
}


//-------
void ofxSpreadsheet::drawSpreadsheet() {
    fboSheet.begin();
    ofClear(0, 0);
    ofBackground(0);
    for (int i=0; i<entries.size(); i++) {
        vector<float> entry = entries[i];
        for (int j=0; j<entry.size(); j++) {
            drawCell(i, j, ofToString(entry[j]));
        }
    }
    fboSheet.end();
}

//-------
void ofxSpreadsheet::drawCell(int row, int col, string cell, CellType type) {
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(col * cellWidth, (1+row) * cellHeight);
    
    if (type == CELL)
        ofSetColor(255);
    else if (type == SELECTED)
        ofSetColor(200);
    else if (type == HEADER)
        ofSetColor(150);
    
    ofFill();
    ofRect(0, 0, cellWidth, -cellHeight);
    
    ofSetColor(0, 60);
    ofSetLineWidth(1);
    ofNoFill();
    ofRect(0, 0, cellWidth, -cellHeight);
    
    ofSetColor(0);
    ofDrawBitmapString(cell, 2, -2);
    
    ofPopMatrix();
    ofPopStyle();
}

//-------
void ofxSpreadsheet::drawScrollBar() {
    scrollHeight = (height - fboHeader.getHeight()) * fboScrollBar.getHeight() / cellsHeight;

    fboScrollBar.begin();
    ofPushStyle();
    ofClear(0, 0);
    ofBackground(220);
    ofFill();
    ofSetColor(150);
    ofRect(0, scrollTop, fboScrollBar.getWidth(), scrollHeight);
    ofPopStyle();
    fboScrollBar.end();
}

//-------
void ofxSpreadsheet::draw(int x, int y) {
    drawPosition.set(x, y+fboHeader.getHeight());
    fboHeader.draw(x, y);
    sheet.draw(x, y+fboHeader.getHeight());
    fboScrollBar.draw(x + sheetWidth, y+fboHeader.getHeight());
    
    /* check if fbo too short to display all entries */
    if (entries.size() * cellHeight > fboSheet.getHeight()) {
        fboSheet.allocate(width-fboScrollBar.getWidth(), (1+entries.size()) * cellHeight);
        drawSpreadsheet();
        highlightRows(selection, true);
    }
}

//-------
ofxSpreadsheet::~ofxSpreadsheet() {
    ofRemoveListener(ofEvents().update, this, &ofxSpreadsheet::update);
    ofRemoveListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
    ofRemoveListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
}