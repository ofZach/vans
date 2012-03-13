#pragma once
#include "ofxOpenCv.h"
#include "trueVectorField.h"
#include "lineUtils.h"
#include "ofxControlPanel.h"
#include "colorTracker.h"
#include "blobTracker.h"

extern ofxControlPanel * guiPtr;
using namespace lineUtils;

class trackerManager : public ofBaseDraws{
	public:
	
		void setup(float w, float h);
		void update( ofxCvColorImage & rgb, ofxCvGrayscaleImage & alpha, ofxCvGrayscaleImage & depth);
		
		float getWidth(){
			return color.getWidth();
		}	
		
		float getHeight(){
			return color.getHeight();
		}
		
		void recordFrames(bool bTrue);
		
		void draw(float x, float y);
		void draw(float x, float y, float w, float h);		
		
		void debugDraw();

		int curNumPts;
		float t1;
						
		string sessionStr;
		float lightSize;
		float averageYPosition; 
		
		float statePct;
		trueVectorField field;
		
		ofxCvColorImage color;
		ofxCvGrayscaleImage alpha; 
		ofxCvContourFinder finder; 
		ofxCvGrayscaleImage depthImage;
		ofxCvGrayscaleImage feetImage;
		ofxCvContourFinder feetFinder;
		
		
		ofImage rgbaPix;
		vector <ofPoint> prePts;		
		
		float timeSincePts;
		float normalizedSmoothedSpeed;
		float smoothedDepth;
		float normalizedSmoothDepth;
		
		int curId;
		
		ofPoint preCentroid; 
		float preArea;
		float smoothedDiff;
    
		blobTracker feetTracker;
		colorTracker  CT ;

		float screenW, screenH;
};