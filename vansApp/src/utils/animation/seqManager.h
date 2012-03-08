
#pragma once

#include "advImageSequence.h"
#include "ofImage.h"

class seqManager{

	public:
		seqManager(){
			frameRate	= 30.0;
			seq			= NULL;
			pct			= 0.0;
			bRev		= false;
			bFlipHoriz	= false;
			bFlipVert	= false;
			drawScale	= 1.0;
		}
		
		void setup( advImageSequence * imgSeq, float fps ){
			seq = imgSeq;
			pct = 0.0;
			frameRate = fps;
			
			if( imgSeq->getNumImages() ){
				width = imgSeq->getFrame(0)->getWidth();
				height = imgSeq->getFrame(0)->getHeight();
			}
			
			seq->setSection(0);
			currentSection = seq->currentSection;
			whichSection = 0;
			frameCounter = currentSection.startFrame;
			timeSince = ofGetElapsedTimef();
			bPlay = false;
			bDone = false;
			numSections = seq->sections.size();
		}
		
		void setScale(float scale){
			drawScale = scale;
		}
		
		void setAnchorPercent(float xPct, float yPct){
			anchor.set(xPct, yPct);
		}
		
		bool lastSection(){
			return (numSections > 0 && (whichSection == numSections-1) );
		}
		
		//CHECK ME FOR REVERSE!!!!!!
		float getPercent(){
			if(  currentSection.endFrame == currentSection.startFrame ) return 0;
			
			float percent = 0.0;
			percent = (float)(float) ( frameCounter - currentSection.startFrame) / fabs ( (float)( currentSection.endFrame - currentSection.startFrame) );
			percent = ofClamp(percent, 0, 1);
			
			if(bRev) percent = 1.0 - percent;
						
			return percent;
		}
		
		int getFrame(){
			if(  currentSection.endFrame == currentSection.startFrame ) return 0;
			
			int cFrame = frameCounter - currentSection.startFrame;
			if(bRev) cFrame = currentSection.endFrame - cFrame;
						
			return cFrame;		
		}
		
		void play(){
			bPlay = true;
		}
		
		void stop(){
			bPlay = false;
		}
		
		void setSection(string sectionName){
			if( seq->setSection(sectionName) ){
				whichSection = seq->whichSection;			
				startNewSection();				
			}
			
		}
		
		void next(){
			if( whichSection < numSections-1 )whichSection++;
			else return;
			
			startNewSection();
		}
		
		void prev(){
			if( whichSection > 0 && numSections > 0)numSections--;
			else return;
			
			startNewSection();
		}
		
		void restart(){
			if( numSections > 0)whichSection = 0;
			else return;
			
			startNewSection();
		}

		void restartAndWait(){
			restart();
			stop();
		}		
		void startNewSection(){
			currentSection = seq->sections[whichSection];
			
			frameCounter = currentSection.startFrame;
			
			if( currentSection.endFrame < currentSection.startFrame ){
				bRev = true;
			}else{
				bRev = false;
			}
			
			timeSince = ofGetElapsedTimef();
			bDone = false;
		}
		
		bool isDone(){
			return bDone;
		}
		
		void setFrameRate(float fps){
			frameRate = fps;
		}
		
		void update(){
			if(seq == NULL)return;
			
			bool bUpdate = false;
			
			if( bRev ){
				if( frameCounter > currentSection.endFrame )bUpdate = true;
			}else{
				if( frameCounter < currentSection.endFrame )bUpdate = true;
			}
			
			if( bPlay && bUpdate ){
				
				float timeDelta = ofGetElapsedTimef() - timeSince;
				
				if( bRev )frameCounter -= timeDelta * frameRate;
				else frameCounter += timeDelta * frameRate;
				
				//printf("frameCounter %f\n", frameCounter);
				
				if( ( bRev && frameCounter <= currentSection.endFrame) || ( !bRev && frameCounter >= currentSection.endFrame ) ){
					if( currentSection.bLoop ){
						frameCounter = currentSection.startFrame;
					}else{
						frameCounter = currentSection.endFrame;
					}
					bDone = true;
				}
						
			}
			timeSince = ofGetElapsedTimef();
		}
		
		ofImage * getCurrentFrame(){
			if( seq == NULL )return NULL;
			ofImage * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			return img;
		}

		void draw(float x, float y, float w, float h){
			if(seq == NULL)return;
			ofImage * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			img->draw(x, y, w, h);
		}
		
		void draw(float x, float y){
			if(seq == NULL)return;
			ofImage * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			
			if( bFlipVert ){
				img->draw(x, y, img->getWidth()*drawScale, -img->getHeight()*drawScale);
			}else if( bFlipHoriz ){			
				img->draw(x, y, -img->getWidth()*drawScale, img->getHeight()*drawScale);
			}else if( bFlipHoriz && bFlipVert){
				img->draw(x, y, -img->getWidth()*drawScale, -img->getHeight()*drawScale);			
			}else img->draw(x, y, img->getWidth()*drawScale, img->getHeight()*drawScale);
			
		}
		
		ofPoint anchor;
		
		float frameRate;
		seqSection currentSection;
		
		float frameCounter;
		float pct;
		float timeSince;
		advImageSequence * seq;
		
		float width, height;
		
		bool bFlipHoriz;
		bool bFlipVert;
		
		int whichSection;
		int numSections;
		
		bool bDone;
		bool bRev;
		
		float drawScale;

		bool bEndThisLoop;
		bool bPlay;
		
};