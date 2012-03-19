#include "ofMain.h"
#include "graphicParticle.h"
#include "genericSequence.h"

class seqParticle : public graphicParticle{
	public:
		
//		virtual void setup(ofPoint p, ofPoint v, float s){
//			unique			= ofRandom(0, 100000);
//			drag			= ofRandom(0.97, 0.988);
//			pos				= p;
//			vel				= v;
//			targetScale		= s;
//			scale			= 0.0;
//			pctScale		= 0.0;
//			pctTargetScale	= 1.0;
//			reached			= false;			
//		}

		virtual void setImageSequence(advImageSequence * seqPtr, ofPoint anchor = ofPoint(0.5, 0.5)){
			seq.setup(seqPtr, 20);
			seq.setAnchorPercent(anchor.x, anchor.y);
			seq.play();
		}
		
		virtual void update(){
			seq.update();
			
			graphicParticle::update();
		}
		
		virtual void draw(){
			seq.pos = pos;
			seq.drawScale = scale;
			seq.draw();
		}
		
		genericSequence seq;
		
//		ofPoint pos;
//		ofPoint vel;
//		ofPoint frc;
//		
//		float drag;
//		float unique;
//		float scale;
};