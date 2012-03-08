#pragma once

#include "genericSequence.h"

class triggerSequence : public genericSequence{
	public:
	
		virtual void setTriggerSound(string triggerSoundKey, float volMin, float volMax, float pitchMin, float pitchMax, float _pan){
			triggerKey = triggerSoundKey;
			vMin = volMin;
			vMax = volMax;
			pMin = pitchMin;
			pMax = pitchMax;
			pan  = _pan;
		}

		virtual void checkInteraction( trackerManager * tracker ){
			
			if( tracker->checkHit(pos, 1.0 ) ){
				if( !bPlay || isDone() ){
					play();
					setRepeatCount(2);
					if( triggerKey != "" ){
						SP.play(triggerKey, ofRandom(vMin, vMax), ofRandom(pMin, pMax), pan);
					}
				}
			}
			
		}
		
			string triggerKey;
			float vMin;
			float vMax;
			float pMin;
			float pMax;
			float pan;

};