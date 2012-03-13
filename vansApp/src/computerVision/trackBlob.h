#pragma once 
#include "ofxOpenCv.h"

class trackBlob{
	public:
	
	trackBlob(){
		b = 0.0;
		id = -1;
		lightAmnt = 0.0;
	}
	
	void cloneFromBlob(ofxCvBlob & b){
		cvBlob.pts = b.pts; 
		cvBlob.centroid = b.centroid;
		cvBlob.area = b.area;
		cvBlob.length = b.length;
		cvBlob.boundingRect = b.boundingRect;
		cvBlob.hole = b.hole;
		cvBlob.nPts = b.nPts;
	}

	void birth(unsigned int newId){
		id = newId;
		timeFirstSeen = ofGetElapsedTimef();
		timeSeen = 0.0;
		b = 1.0;
		bMatched = true;
	}
	
	void keepAlive(){
		timeSeen = ofGetElapsedTimef() - timeFirstSeen;
		bMatched = true;
		b = 1.0;		
	}
	
	void debugDraw(){
		ofSetColor(50, 90, 250);
		ofCircle(cvBlob.centroid, 4);
		
		string str = "id: " + ofToString(id);
		str += "\nspeed: " + ofToString(speed);
		str += "\ntime seen: " + ofToString(timeSeen);
		
		ofSetColor(255);
		ofLine(cvBlob.centroid, cvBlob.centroid + speed * 10.0);
		
		ofDrawBitmapString(str, cvBlob.boundingRect.x, cvBlob.boundingRect.y);
	}
	
	ofxCvBlob cvBlob;
	
	ofPoint speed;
	float lightAmnt;
	float dist;
	bool bMatched;
	unsigned int id;
	float timeFirstSeen;
	float timeSeen;
	float b;
};