#include "genericSequence.h"

void genericSequence::loadSequence(string sequencePath, ofPoint _pos,  float fps,  ofPoint anchorPercent, float scale){
	originalPath = sequencePath;
	
	pos = _pos;
	seqPtr = &seq;			
	seqPtr->loadSeq(sequencePath, "png");
	seqManager::setup(seqPtr, fps);
	seqManager::setAnchorPercent(anchorPercent.x, anchorPercent.y);
	repeatCount = 0;
	sleepTime   = 0;
	sleepTillTime = 0;
	timerF     = 0;
	bNeedsDelay	= false;
	
	setScale(scale);
}

void genericSequence::loadSequence(advImageSequence * _seqPtr, ofPoint _pos,  float fps,  ofPoint anchorPercent, float scale){
	originalPath = "";
	
	pos = _pos;
	seqPtr = _seqPtr;			
	seqManager::setup(seqPtr, fps);
	seqManager::setAnchorPercent(anchorPercent.x, anchorPercent.y);
	repeatCount = 0;
	sleepTime   = 0;
	sleepTillTime = 0;
	timerF     = 0;
	bNeedsDelay	= false;
	
	setScale(scale);
}


void genericSequence::setRepeatCount(int count, float _sleepTime){
	repeatCount		= MAX(0, count-1);
	sleepTime		= _sleepTime;
	sleepTimeMin	= -1;
	sleepTimeMax	= -1;	
	timerF			= -1000.0f;
	bNeedsDelay		= false;	
}

void genericSequence::setRepeatCount(int count, float _sleepTimeMin, float _sleepTimeMax){
	repeatCount		= MAX(0, count-1);
	sleepTimeMin	= _sleepTimeMin;
	sleepTimeMax	= _sleepTimeMax;	
	timerF			= -1000.0f;
	bNeedsDelay		= false;	
}

void genericSequence::update(float timeStep){
	seqManager::update();
	
	if( seqManager::isDone() && repeatCount >= 1){
			
		if( timerF == -1000.0f ){
			if( sleepTimeMin != -1 ){
				timerF = ofRandom(sleepTimeMin, sleepTimeMax);			
			}else{
				timerF = sleepTime;
			}
		}
		
		if( timerF <= 0.0 ){
			repeatCount--;
			setSection(currentSection.name);
			timerF = -1000.0f;
		}else{
			timerF -= ofGetLastFrameTime();
		}
	}
}

bool genericSequence::isDone(){
	if( seqManager::isDone() && ofGetElapsedTimef() > sleepTillTime ){
		return true;
	}
	return false;
}

void genericSequence::setSleepAtEndTime(float sleepLengthSecs){
	sleepTillTime = ofGetElapsedTimef() + sleepLengthSecs;
}

void genericSequence::draw(){

	if( fabs(rotateRange) > 0.0 || pos.z != 0.0 ){
		ofPushMatrix();
			ofTranslate(pos.x, pos.y, pos.z);
			if( rotateRange != 0.0 ){
				ofRotateZ( ofSignedNoise( ofGetElapsedTimef() * rate, 1000.0f ) * rotateRange );
			}
			seqManager::draw(0.0, 0.0);
		ofPopMatrix();
	}else{
		seqManager::draw(pos.x, pos.y);
	}
	
	//baseLayer::debugDraw();
}

void genericSequence::draw(float x, float y, float w, float h){

	if( fabs(rotateRange) > 0.0 ){
		ofPushMatrix();
			ofTranslate(x, y, 0.0);
			if( rotateRange != 0.0 ){
				ofRotateZ( ofSignedNoise( ofGetElapsedTimef() * rate, 1000.0f ) * rotateRange );
			}
			seqManager::draw(0.0, 0.0, w, h);
		ofPopMatrix();
	}else{
		seqManager::draw(pos.x, pos.y, w, h);
	}
	
	//baseLayer::debugDraw();
}