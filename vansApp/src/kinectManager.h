
#pragma once
#include "liveInput.h"
#include "ofxControlPanel.h"

extern bool bLaptop;

class kinectManager{


	public:
	
		void setup(ofxControlPanel & panel){
			guiPtr = &panel; 
			
			numDevices = 0;
			
			if( bLaptop == false ){
				ofxKinect tmpKinect;
				tmpKinect.init(true, false, false);			
				numDevices = ofxKinect::numTotalDevices();
			}
			
			cout << " this many devices " << numDevices << endl; 
	
			if( numDevices >= 1 ){
				input.setupKinect(0, LIVE_MODE_KINECT_REG);
			}else{
				input.setupKinectSequence("testRecordings/rec-2012-03-07-12-37-27-299/", LIVE_MODE_KINECT_REG);
			}
			
//			input.bSmooth = true;
//			inputR.bSmooth = true;
		}
		
		ofxCvColorImage & getColor(){
			return color;
		}
		
		ofxCvGrayscaleImage & getSmoothedAlpha(){
			return alphaSmoothed; 
		}

		ofxCvGrayscaleImage & getDepth(){
			return alphaOg; 
		}

		void update(){
			
			if( numDevices && ofGetFrameNum() > 10 && ofGetFrameNum() % 20 == 0 ){
				input.kinect.setClippedDataToZero(guiPtr->getValueB("bFilterWhitePixels"));
				input.kinect.setDepthClipping(guiPtr->getValueF("clipMin") * 10, guiPtr->getValueF("clipMax") * 10	); 
			}
			
			bool bFlipH = guiPtr->getValueB("flipVideoH"); 
			bool bFlipV = guiPtr->getValueB("flipVideoV");
			if( numDevices == 1  ){
				bFlipH = !bFlipH;
			}
			
			if( input.update(bFlipH, bFlipV, false ) ){
				bFrameLNew = true;
				
				if( color.getWidth() == 0 ){
					color.allocate(640, 480);
					alphaSmoothed.allocate(640, 480);
					alphaOg.allocate(640, 480);
				}
							
				color			= input.color;
				alphaSmoothed	= input.preview;
				alphaOg			= alphaSmoothed;
								
				if( guiPtr->getValueF("videoBrightness") != 0.0 ){
					
					unsigned char * pix = color.getPixels();
					int numPix = 3 * color.getWidth() * color.getHeight();
					float val = 0.0;
					float brightnessAmnt = guiPtr->getValueF("videoBrightness");
					
					for(int i = 0; i < numPix; i++){
						val = pix[i];
						val += brightnessAmnt;
						if( val > 255 )val = 255;
						else if( val < 0 ) val = 0;
						pix[i] = val;
					}
					
					color.flagImageChanged();
				}
				
			}else{
				bFrameLNew = false;			
			}
	
		}
		
		bool isFrameNew(){
			return bFrameLNew;
		}
		
		string sessionStr;
		int numDevices;
		bool bFrameLNew;
		liveInput input;

		
		ofxCvGrayscaleImage alphaSmoothed;
		ofxCvColorImage color; 

		ofxCvGrayscaleImage alphaOg;

		ofxCvGrayscaleImage alphaV;
		ofxCvColorImage colorV; 
		
		ofxControlPanel * guiPtr;
		
		//armTracker armTrackR;


};