#pragma once

#include "ofMain.h"
#include "ofxSpreadsheet.h"

struct myStruct {
    
    myStruct(int _entryID,int _customerID,string _customerFirstname,string _customerSurname,float _balance) {
        entryID = _entryID;
        customerID = _customerID;
        customerFirstname = _customerFirstname;
        customerSurname = _customerSurname;
        balance = _balance;
    }
    
    vector <string> getDetails() {
        vector<string> detail;
        detail.push_back(ofToString(entryID));
        detail.push_back(ofToString(customerID));
        detail.push_back(customerFirstname);
        detail.push_back(customerSurname);
        detail.push_back(ofToString(balance));
        return detail;
    }
    
    int entryID;
    int customerID;
    string customerFirstname;
    string customerSurname;
    float balance;
};
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofxSpreadsheet spreadsheet;
        bool reverse;
        vector <myStruct> details;
};
