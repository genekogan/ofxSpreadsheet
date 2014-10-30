#include "ofxSpreadsheet.h"

//-------
void ofxSpreadsheetScrollable::load(ofFbo *text, float w, float h) {
    this->fbo = text;
    fbo->begin();
    ofClear(0, 0);
    fbo->end();
    ofFbo::allocate(w, h, GL_RGBA32F_ARB);
    ofFbo::begin();
    ofClear(0, 0);
    ofFbo::end();
}

//-------
void ofxSpreadsheetScrollable::update() {
    ofPushStyle();
    ofFbo::begin();
    ofClear(0, 0);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofSetColor(255);
    fbo->draw(position.x, position.y);
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
    scrollbarWidth = 12;
    scrollbarHeight = 12;
    cellHeight = 20;
    
    draggingV = false;
    selectMultiple = false;
    cmd = false;
    
    fboHeader.allocate(width-fboScrollV.getWidth(), headerHeight);
    fboScrollV.allocate(scrollbarWidth, height-fboHeader.getHeight());
    fboScrollH.allocate(width-fboScrollV.getWidth(), scrollbarHeight);
    fboSheet.allocate(width-fboScrollV.getWidth(), height-fboHeader.getHeight());
    
    sheetWidth = width-fboScrollV.getWidth();
    sheetHeight = height-fboScrollH.getHeight()-fboHeader.getHeight();
    sheet.load(&fboSheet, sheetWidth, sheetHeight);
    sheetHeader.load(&fboHeader, sheetWidth, headerHeight);
    
    drawScrollBarV();
    drawScrollBarH();
    drawSpreadsheet();
    
    setInputsActive(true);
}

//-------
void ofxSpreadsheet::setHeaders(vector<string> headers) {
    clear();
    fboHeader.allocate(width-fboScrollV.getWidth(), headerHeight);
    fboScrollV.allocate(scrollbarWidth, height-fboHeader.getHeight());
    fboScrollH.allocate(width-fboScrollV.getWidth(), scrollbarHeight);
    fboSheet.allocate(width-fboScrollV.getWidth(), height-fboHeader.getHeight());
    
    this->headers = headers;
    cellWidth = (float) fboSheet.getWidth() / headers.size();
    if (cellWidth < 100)    cellWidth = MIN_CELL_WIDTH;
    cellsWidth = cellWidth * headers.size();
    
    entries.clear();
    drawHeader();
    sheetHeader.update();
    drawScrollBarH();
    drawScrollBarV();
    checkBounds();
}

//-------
void ofxSpreadsheet::highlightColumn(int column) {
    highlightedColumns.push_back(column);
}

//-------
void ofxSpreadsheet::addEntry(vector<float> entry) {
    entries.push_back(entry);
    checkBounds();
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
    drawScrollBarV();
    sheet.update();
    
    bool b;
    ofNotifyEvent(changeEvent, b, this);
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
    
    /* scroll from keyboard */
    scrollTop = max(scrollTop, (float) row * 7 - height + fboHeader.getHeight());
    sheet.setVertical(-scrollTop * cellsHeight / fboScrollV.getHeight());
    sheet.update();
    drawScrollBarV();
}

//-------
void ofxSpreadsheet::selectAllRows() {
    selection.clear();
    for (int r=0; r<entries.size(); r++) {
        selection.push_back(r);
    }
    highlightRows(selection, true);
}

//-------
void ofxSpreadsheet::deleteSelectedRows() {
    entries.erase(entries.begin() + selection[0], entries.begin() + selection[selection.size()-1] + 1);
    drawSpreadsheet();
    cellsHeight = entries.size() * cellHeight;
    scrollHeight = (height - fboHeader.getHeight() - fboScrollH.getHeight()) * height / cellsHeight;
    scrollTop = ofClamp(scrollTop, 0, abs(height - scrollHeight - fboScrollH.getHeight()));
    sheet.setPosition(0, -scrollTop * cellsHeight / fboScrollV.getHeight());
    
    if (entries.size() > 0) {
        int r = min((int) selection[0], (int) entries.size()-1);
        selection.clear();
        selection.push_back(r);
        highlightRows(selection, true);
    }
    else {
        selection.clear();
    }
    drawScrollBarV();
    sheet.update();
    
    bool b;
    ofNotifyEvent(changeEvent, b, this);
}

//-------
void ofxSpreadsheet::clear() {
    if (entries.size() == 0)    return;
    selectAllRows();
    deleteSelectedRows();
    bool b;
    ofNotifyEvent(changeEvent, b, this);

}

//-------
void ofxSpreadsheet::keyPressed(ofKeyEventArgs &evt) {
    if (entries.size() == 0)    return;
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
    else if (evt.key == OF_KEY_COMMAND) {
        cmd = true;
    }
    else if (evt.key == 'a' || evt.key == 'A') {
        if (cmd) {
            selectAllRows();
        }
    }
}

//-------
void ofxSpreadsheet::keyReleased(ofKeyEventArgs &evt) {
    if (evt.key == OF_KEY_SHIFT) {
        selectMultiple = false;
    }
    else if (evt.key == OF_KEY_COMMAND) {
        cmd = false;
    }
}

//-------
void ofxSpreadsheet::mousePressed(ofMouseEventArgs &evt){
    ofRectangle sheetRect(drawPosition.x, drawPosition.y,
                          sheetWidth, sheetHeight);
    ofRectangle barV(drawPosition.x + sheetWidth,
                     drawPosition.y + scrollTop,
                     fboScrollV.getWidth(), scrollHeight);
    
    ofRectangle barH(drawPosition.x + scrollLeft,
                     drawPosition.y + sheetHeight,
                     scrollWidth, fboScrollH.getHeight());
    
    if (sheetRect.inside(evt.x, evt.y)){
        float position = evt.y - drawPosition.y - sheet.getPosition().y;
        int row = floor(position / cellHeight);
        if (row < entries.size())
            selectRow(row);
    }
    else if (barV.inside(evt.x, evt.y)) {
        mouseHold.set(evt.x, evt.y);
        draggingV = true;
    }
    else if (barH.inside(evt.x, evt.y)) {
        mouseHold.set(evt.x, evt.y);
        draggingH = true;
    }
    
}

//-------
void ofxSpreadsheet::mouseDragged(ofMouseEventArgs &evt){
    if (draggingV && scrollHeight < cellsHeight) {
        scrollTop = ofClamp(scrollTop + (evt.y - mouseHold.y), 0, height - scrollHeight - fboScrollH.getHeight());
        sheet.setVertical(-scrollTop * cellsHeight / fboScrollV.getHeight());
        mouseHold.set(mouseHold.x, evt.y);
        drawScrollBarV();
        sheet.update();
    }
    else if (draggingH && scrollWidth < cellsWidth) {
        scrollLeft = ofClamp(scrollLeft + (evt.x - mouseHold.x), 0, width - scrollWidth - fboScrollV.getWidth());
        sheet.setHorizontal(-scrollLeft * cellsWidth / fboScrollH.getWidth());
        sheetHeader.setHorizontal(-scrollLeft * cellsWidth / fboScrollH.getWidth());
        mouseHold.set(evt.x, mouseHold.y);
        drawScrollBarH();
        sheetHeader.update();
        sheet.update();
    }
}

//-------
void ofxSpreadsheet::mouseReleased(ofMouseEventArgs &evt){
    draggingV = false;
    draggingH = false;
}

//-------
void ofxSpreadsheet::drawSpreadsheet() {
    scrollWidth = (width - fboScrollV.getWidth()) * fboScrollH.getWidth() / cellsWidth;
    scrollHeight = (height - fboHeader.getHeight()) * height / cellsHeight;
    
    fboSheet.begin();
    ofClear(0, 0);
    ofSetColor(255);
    ofRect(0, 0, fboSheet.getWidth(), fboSheet.getHeight());
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
    bool isHighlightedColumn = false;
    for (int i=0; i<highlightedColumns.size(); i++) {
        if (highlightedColumns[i] == col) {
            isHighlightedColumn = true;
            break;
        }
    }
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(col * cellWidth, (1+row) * cellHeight);
    
    if (type == CELL) {
        if (isHighlightedColumn)
            ofSetColor(225);
        else
            ofSetColor(255);
    }
    else if (type == SELECTED) {
        if (isHighlightedColumn)
            ofSetColor(180);
        else
            ofSetColor(200);
    }
    else if (type == HEADER) {
        ofSetColor(150);
    }
    
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
void ofxSpreadsheet::drawHeader() {
    fboHeader.begin();
    ofClear(0, 0);
    for (int i=0; i<headers.size(); i++) {
        drawCell(0, i, headers[i], HEADER);
    }
    fboHeader.end();
}

//-------
void ofxSpreadsheet::drawScrollHeader() {
}


//-------
void ofxSpreadsheet::drawScrollBarV() {
    scrollHeight = (height - fboHeader.getHeight()) * fboScrollV.getHeight() / cellsHeight;
    fboScrollV.begin();
    ofPushStyle();
    ofClear(0, 0);
    ofSetColor(220);
    ofRect(0, 0, fboScrollV.getWidth(), fboScrollV.getHeight());
    ofFill();
    ofSetColor(150);
    ofRect(0, scrollTop, fboScrollV.getWidth(), scrollHeight);
    ofPopStyle();
    fboScrollV.end();
}

//-------
void ofxSpreadsheet::drawScrollBarH() {
    scrollWidth = (width - fboScrollV.getWidth()) * fboScrollH.getWidth() / cellsWidth;
    fboScrollH.begin();
    ofPushStyle();
    ofClear(0, 0);
    ofSetColor(220);
    ofRect(0, 0, fboScrollH.getWidth(), fboScrollH.getHeight());
    ofFill();
    ofSetColor(150);
    ofRect(scrollLeft, 0, scrollWidth, fboScrollH.getHeight());
    ofPopStyle();
    fboScrollH.end();
}

//-------
void ofxSpreadsheet::draw(int x, int y) {
    drawPosition.set(x, y+fboHeader.getHeight());
    sheet.draw(x, y+fboHeader.getHeight());
    sheetHeader.draw(x, y);
    fboScrollV.draw(x + sheetWidth, y+fboHeader.getHeight());
    fboScrollH.draw(x, y+fboHeader.getHeight()+sheetHeight);
    checkBounds();
}

//-------
void ofxSpreadsheet::checkBounds() {
    if (entries.size() * cellHeight > fboSheet.getHeight()) {
        fboSheet.allocate(width-fboScrollV.getWidth(), (12+entries.size()) * cellHeight);
        drawSpreadsheet();
        highlightRows(selection, true);
    }
    if (headers.size() * cellWidth > fboSheet.getWidth()) {
        fboHeader.allocate((1+headers.size())*cellWidth-fboScrollV.getWidth(), headerHeight);
        fboSheet.allocate((1+headers.size())*cellWidth-fboScrollV.getWidth(), (12+entries.size()) * cellHeight);
        drawHeader();
        drawSpreadsheet();
        highlightRows(selection, true);
    }
}

void ofxSpreadsheet::setInputsActive(bool inputsActive) {
    this->inputsActive = inputsActive;
    if (inputsActive) {
        ofAddListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &ofxSpreadsheet::keyReleased);
        ofAddListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
        ofAddListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
    }
    else {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxSpreadsheet::keyPressed);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxSpreadsheet::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxSpreadsheet::mouseReleased);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxSpreadsheet::mouseDragged);
    }
}

//-------
ofxSpreadsheet::~ofxSpreadsheet() {
    if (inputsActive) {
        setInputsActive(false);
    }
}