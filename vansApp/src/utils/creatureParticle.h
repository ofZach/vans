#pragma once
#include "ofMain.h"

class creatureParticle{
	public:

		virtual void init(ofPoint _pos, ofPoint _vel, float _dragAmnt = ofRandom(0.9, 0.95), float _alpha = 1.0, float _scale = ofRandom(0.8, 1.1) ){
			pos			= _pos;
			vel			= _vel;
			dragAmnt	= _dragAmnt;
			alpha		= _alpha;
			scale		= _scale;	
			uniqueID	= ofRandom(0, 1000000);
		}
		
		void setPos(float x, float y){
			pos.set(x, y);
		}

		void setPos(ofPoint pt){
			pos = pt;
		}
		
		void setPos(float x, float y, float z){
			pos.set(x, y, z);
		}
		
		ofPoint getPos(){
			return pos;
		}
		
				
		virtual void update(float updateSpeed = 1.0){};
		virtual void draw() = 0;
		
		ofPoint ogPos;
		ofPoint pos;
		ofPoint vel;

		float scale;
		float ogScale;
	protected:
		
		ofPoint target;
		
		float alpha;
		float dragAmnt;
		float uniqueID;
		
};