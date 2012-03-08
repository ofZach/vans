#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxControlPanel.h"
#include "liveInput.h"

#include "vansApp.h"
#include "sceneViewer.h"
#include "trackerManager.h"
#include "ofxAssimpModelLoader.h"
#include "kinectManager.h"

class testApp : public ofBaseApp{
	
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
		void windowResized(int w, int h);
		
		void dragEvent(ofDragInfo dragInfo);
		void eventsIn(guiCallbackData & data);
		
		float elapsedTime;
		int appFrameCount;
		float appFrameRate;
		
		sceneViewer viewer;
				
		ofxCvContourFinder contourFinder;
		trackerManager tracker;
		
		ofxKinect kinect; 
		
		kinectManager km;
		
		vansApp vans;
				
		float speedComp;
										
		int capW, capH;
				
		ofxControlPanel gui;		
};
	
