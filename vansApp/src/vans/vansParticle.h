#pragma once
#include "ofMain.h"

class vansParticle{
	public:
		
		virtual void setup(ofPoint p, ofPoint v, float s){
			unique = ofRandom(0, 100000);
			drag   = ofRandom(0.97, 0.988);
			pos	   = p;
			vel	   = v;
			scale  = s;
		}
		
		virtual void update(){
			
		}
		
		virtual bool shouldKill(){
			return false;
		}
		
		virtual void draw(){
		
		}
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag;
		float unique;
		float scale;
};