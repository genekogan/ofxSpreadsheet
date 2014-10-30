#pragma once

#include "ofMain.h"


//-----------
class ofxSpreadsheetScrollable : public ofFbo {
public:
    void load(ofFbo *text, float w, float h);
    void update();
    void setPosition(float x, float y) {position.set(x, y);}
    void setHorizontal(float x) {position.x = x;}
    void setVertical(float y) {position.y = y;}
    ofPoint getPosition() {return position;}
private:
    ofFbo *fbo;
    ofPoint position;
};


//-----------
class ofxSpreadsheet
{
public:
    enum CellType { HEADER, CELL, SELECTED };
    
    ~ofxSpreadsheet();
    void setup(int sheetWidth, int sheetHeight);
    void setHeaders(vector<string> headers);
    void highlightColumn(int column);
    
    void addEntry(vector<float> entry);
    void selectRow(int row);
    void deleteSelectedRows();
    void clear();
    
    void draw(int x, int y);
    void setInputsActive(bool inputsActive);
    
    int getNumberOfEntries() { return entries.size(); }
    vector<vector<float> > & getEntries() { return entries; }
    
    template<typename ListenerClass, typename ListenerMethod>
    void addSpreadsheetChangedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(changeEvent, listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void removeSpreadsheetChangedListener(ListenerClass *listener, ListenerMethod method) {
        ofRemoveListener(changeEvent, listener, method);
    }

    
private:
    
    int MIN_CELL_WIDTH = 100;
    
    void keyPressed(ofKeyEventArgs &evt);
    void keyReleased(ofKeyEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    void highlightRows(vector<int> rows, bool highlight);
    void selectAllRows();
    
    void drawCell(int row, int col, string cell, CellType type = CELL);
    void drawSpreadsheet();
    void drawHeader();
    void drawScrollHeader();
    void drawScrollBarV();
    void drawScrollBarH();
    void checkBounds();
    
    vector<vector<float> > entries;
    vector<string> headers;
    vector<int> selection;
    vector<int> highlightedColumns;
    
    int width, height;
    int sheetWidth, sheetHeight;
    float cellWidth, cellHeight;
    
    ofFbo fboSheet, fboHeader, fboScrollV, fboScrollH;
    ofxSpreadsheetScrollable sheet, sheetHeader;
    ofPoint drawPosition;
    
    int headerHeight, scrollbarWidth, scrollbarHeight;
    float scrollTop, scrollHeight, cellsHeight;
    float scrollLeft, scrollWidth, cellsWidth;
    ofPoint mouseHold;
    bool draggingV, draggingH;
    bool selectMultiple;
    bool cmd;
    bool inputsActive;
    
    ofEvent<bool> changeEvent;
};

