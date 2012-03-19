#pragma once
#include "ofMain.h"
#include "vansParticle.h"

class graphicParticle : public vansParticle{
	public:
		
		virtual void setup(ofPoint p, ofPoint v, float s){
			unique			= ofRandom(0, 100000);
			drag			= ofRandom(0.97, 0.988);
			pos				= p;
			vel				= v;
			targetScale		= s;
			scale			= 0.0;
			pctScale		= 0.0;
			pctTargetScale	= 1.0;
			reached			= false;			
		}
		
		virtual void setImage(ofImage * imgPtr){
			img = imgPtr;
		}
		
		virtual void update(){
			vel *= drag;
			pos += vel;
			
			if( !reached ){
				pctScale = ofLerp(pctScale, pctTargetScale, 0.24);
			}else{
				pctScale = ofLerp(pctScale, pctTargetScale, 0.1);
			}
			
			if( fabs( scale - targetScale ) < 0.03 ){
				pctTargetScale = 0.0;
				reached = true;
			}
	
			scale = powf(pctScale, 3) * targetScale;
		}
		
		virtual bool shouldKill(){
			return ( reached && scale < 0.05 );
		}
		
		virtual void draw(){
			img->draw(pos.x, pos.y, img->getWidth() * scale, img->getHeight() * scale);
		}
		
		float pctScale;
		float pctTargetScale;
		
		bool reached;
		float targetScale;
		ofImage * img;
		
//		ofPoint pos;
//		ofPoint vel;
//		ofPoint frc;
//		
//		float drag;
//		float unique;
//		float scale;
};