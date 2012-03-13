#pragma once 
#include "ofxOpenCv.h"
#include "Graph.h"


class trackBlob{
    
	public:
	
	trackBlob(){
		b = 0.0;
		id = -1;
		lightAmnt = 0.0;
        
        graphs[0].setup("speed", 1.98);
        graphs[1].setup("x change", 0).setBidirectional(true);
        graphs[2].setup("y change", 0).setBidirectional(true);
        graphs[3].setup("z change", 0);
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
        
        prevSize = size = cvBlob.area;
	}
	
	void keepAlive(){
        
        size = cvBlob.area;
        
		timeSeen = ofGetElapsedTimef() - timeFirstSeen;
		bMatched = true;
		b = 1.0;	
        
        graphs[0].addSample(speed.length());
        graphs[1].addSample(speed.x);
        graphs[2].addSample(speed.y);
        graphs[3].addSample( size - prevSize);
        
        prevSize = size;
        
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
        
        //speedGraph.draw(cvBlob.centroid.x, cvBlob.centroid.y + 100);
        
	}
    
    
    Graph graphs[4];
    
    
	
	ofxCvBlob cvBlob;
	
    float prevSize;
    float size;
    
	ofPoint speed;
	ofPoint preSpeed;
	float lightAmnt;
	float dist;
	bool bMatched;
	unsigned int id;
	float timeFirstSeen;
	float timeSeen;
	float b;
};