#pragma once

#include "ofMain.h"


//-----------
class ofxSpreadsheetScrollable : public ofFbo {
public:
    void load(ofFbo *text, float w, float h);
    void update();
    void setPosition(float position) {this->position = position;}
    float getPosition() {return position;}
private:
    ofFbo *fbo;
    float position;
};


//-----------
class ofxSpreadsheet
{
public:
    enum CellType { HEADER, CELL, SELECTED };
    
    ~ofxSpreadsheet();
    void setup(int sheetWidth, int sheetHeight);
    void setHeaders(vector<string> headers);

    void addEntry(vector<float> entry);
    void selectRow(int row);
    void deleteSelectedRows();
    void clear();
    
    void draw(int x, int y);
    
    int getNumberOfEntries() { return entries.size(); }
    
private:

    void keyPressed(ofKeyEventArgs &evt);
    void keyReleased(ofKeyEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);

    void highlightRows(vector<int> rows, bool highlight);
    void selectAllRows();
    
    void drawCell(int row, int col, string cell, CellType type = CELL);
    void drawSpreadsheet();
    void drawScrollBar();
    
    vector<vector<float> > entries;
    vector<string> headers;
    vector<int> selection;

    int width, height;
    int sheetWidth, sheetHeight;
    float cellWidth, cellHeight;
    
    ofFbo fboSheet, fboHeader, fboScrollBar;
    ofxSpreadsheetScrollable sheet;
    ofPoint drawPosition;
    
    int headerHeight, scrollbarWidth;
    float scrollTop, scrollHeight, cellsHeight;
    ofPoint mouseHold;
    bool dragging;
    bool selectMultiple;
    bool cmd;
};

