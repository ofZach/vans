#pragma once

#include "ofMain.h"
#include "seqManager.h"
#include "advImageSequence.h"
#include "baseLayer.h"

class genericSequence : public seqManager, public baseLayer{
	public:
	
		genericSequence(){
			rotateRange = 0.0;
			originalPath ="";
			seqPtr = &seq;			
		}
		
		virtual void eventsIn(layerMessage & data){
		
		}
		
		void setRepeatCount(int count, float _sleepTime = 0.0);
		void setRepeatCount(int count, float _sleepTimeMin, float _sleepTimeMax);

		
		void setSleepAtEndTime(float sleepLengthSecs);
		bool isDone();
		void loadSequence(string sequencePath, ofPoint pos, float fps = 30.0f, ofPoint anchorPercent = 0.0, float scale = 1.0);
		void loadSequence(advImageSequence * _seqPtr, ofPoint _pos,  float fps = 30.0f,  ofPoint anchorPercent = 0.0, float scale = 1.0);
		
		string getSequencePath(){
			return originalPath;
		}		
		
		void setRotateAmnt(float _rotateRangeDeg, float _rate){
			rotateRange = _rotateRangeDeg;
			rate		= _rate;
		}
		
		void update(float timeStep = 1.0);
		void draw();
		void draw(float x, float y, float w, float h);
		
		float rotateRange, rate;
		bool bNeedsDelay;
		float timerF;
		float sleepTillTime;
		float sleepTime;
		float sleepTimeMin;
		float sleepTimeMax;
		string originalPath;
		advImageSequence seq;
		advImageSequence * seqPtr;
		int repeatCount;

};