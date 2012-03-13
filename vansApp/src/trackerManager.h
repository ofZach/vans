#pragma once
#include "ofxOpenCv.h"
#include "trueVectorField.h"
#include "lineUtils.h"
#include "ofxControlPanel.h"

extern ofxControlPanel * guiPtr;

using namespace lineUtils;

typedef enum{
	TRACKER_NONE_LONG = 0,
	TRACKER_NONE_SHORT,
	TRACKER_LOW,
	TRACKER_MED,
	TRACKER_HIGH
}trackerState;

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
		
		trackerState state;
		float statePct;
		trueVectorField field;
		
		ofxCvColorImage color;
		ofxCvGrayscaleImage alpha; 
		ofxCvContourFinder finder; 
		ofxCvGrayscaleImage depthImage;
		
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

		float screenW, screenH;
};