#include "ofMain.h"
#include "vansParticle.h"
#include "imageResource.h"

class graphicParticle : public vansParticle{
	public:
		
		virtual void setup(ofPoint p, ofPoint v, float s){
			unique = ofRandom(0, 100000);
			drag   = ofRandom(0.97, 0.988);
			pos	   = p;
			vel	   = v;
			targetScale  = s;
			scale = 0.0;
		}
		
		virtual void setImage(string image){
			img.loadImage(image, false, false);
		}
		
		virtual void update(){
			vel *= drag;
			pos += vel;
			scale = ofLerp(scale, targetScale, 0.1);
			if( fabs( scale - targetScale ) < 0.03 ){
				targetScale = 0.0;
			}
		}
		
		virtual void draw(){
		
		}
		
		float targetScale;
		imageResource img;
		
//		ofPoint pos;
//		ofPoint vel;
//		ofPoint frc;
//		
//		float drag;
//		float unique;
//		float scale;
};