#include "ofMain.h"
#pragma once

class layerMessage{
	public:
		string message;
		void * ptr; 
		vector <float> floatVals;
		ofPoint p1, p2, p3;
};

extern ofEvent <layerMessage> globalEvent;
