#include "liveInput.h"

//------------------------------------------------------------------------------------------------
liveInput::liveInput(){
	nearThreshold = 200;
	farThreshold  = 20;
	threshold	  = 17;
	minMaskY	  = 0;
	maxMaskY	  = 60000;
	minMaskX	  = 0;
	maxMaskX	  = 60000;
	doWarp		  = false;
	bgMode		  = BG_MODE_ABS;
	bSmooth		  = false;
	timeToWaitTillNext = 0.0;
}

//call after you setup the mode
//------------------------------------------------------------------------------------------------
void liveInput::setupInputWarp(string xmlName, bool bColor, bool autoSave){
	warper.setup(xmlName, w, h, bColor, autoSave);
	doWarp = true;
}

//------------------------------------------------------------------------------------------------
void liveInput::setupKinect(int device, liveInputMode inputMode){

//	kinect.init(false, true, true);
//	kinect.setRegistration(true);
//	kinect.open(device);
//	kinect.setClippedDataToZero(true);
//	kinect.close();
//	kinect.open(device);
//	kinect.setClippedDataToZero(true);
//	kinect.close();		
//	ofSleepMillis(200);
	
	kinect.init(false, true, true);
	kinect.setRegistration(true);
	kinect.open(device);
	kinect.setClippedDataToZero(true);
		
	w = 640;
	h = 480;
	
//	farThreshold	= initialFar;
//	nearThreshold = initialNear;
	
	this->inputMode = inputMode;
	inputType = LIVE_TYPE_KINECT;
	
	setupMode();
}

//------------------------------------------------------------------------------------------------
void liveInput::setupKinectSequence(string folderPath, liveInputMode inputMode){

	advSeq.loadSeq(folderPath, "png");
	seq.setup(&advSeq,4);
	seq.currentSection.bLoop = true;
	seq.play();
	seq.update();
		
	w = seq.getCurrentFrame()->getWidth();
	h = seq.getCurrentFrame()->getHeight();
	
	this->inputMode = inputMode;
	inputType = LIVE_TYPE_KINECT_SEQ;
	
	setupMode();
}

//------------------------------------------------------------------------------------------------
void liveInput::setupFake(int width, int height, int numPeople, liveInputMode inputMode ){
	w = width;
	h = height;
	
	fakes.assign(numPeople, ofPoint());
	for(int k = 0; k < fakes.size(); k++){
		fakes[k].x  = ofRandom(-200, w+200);
		if( fakes[k].x > w/2 ){
			fakes[k].x = ofRandom(w+10, w + 200 );
		}else{
			fakes[k].x = ofRandom(-20, - 200 );		
		}
		fakes[k].y = ofRandom(0, h);
	}
	
	inputType = LIVE_TYPE_FAKE;	
	this->inputMode = inputMode;
	
	setupMode();
}

//------------------------------------------------------------------------------------------------
void liveInput::setupMode(){
	color.allocate(w, h);
	color2.allocate(w, h);

	grayscale.allocate(w, h);
	inter.allocate(w,h);
	preview.allocate(w, h);
	grayscalePre.allocate(w, h);
	grayscaleCur.allocate(w, h);
	
	if( inputMode == LIVE_MODE_DIFF || inputMode == LIVE_MODE_BG_SUB || inputMode == LIVE_MODE_KINECT || inputMode == LIVE_MODE_KINECT_DEPTH ){
		
		if( inputMode == LIVE_MODE_KINECT_DEPTH ){
			extra1.allocate(w/8, h/8);	
			//extra2.allocate(w, h);	
		}
		
		if( inputType == LIVE_TYPE_KINECT && inputMode == LIVE_MODE_BG_SUB ){
			bgFloat.allocate(w, h);
			bgFloat.setNativeScale(0, 255.0f);
			liveFloat.allocate(w, h);
			liveFloat.setNativeScale(0, 255.0f);
		}
		
	}else if( inputMode == LIVE_MODE_GREENSCREEN ){
		chroma.setup(w, h);
	}

}

//------------------------------------------------------------------------------------------------
void liveInput::setupDC1394( int width, int height, liveInputMode inputMode){

//	w = width;
//	h = height;
//
//	inputType = LIVE_TYPE_1394;
//	this->inputMode = inputMode;
//	
//	ofSetLogLevel(OF_LOG_VERBOSE);
//	
//	dcCamera.setImageType(OF_IMAGE_COLOR);
//	
//	dcCamera.setSize(1280, 960);
//	dcCamera.set1394b(true);
//	//dcCamera.setFormat7(true, 2);
//	
//	//dcCamera.setBayerMode(DC1394_COLOR_FILTER_RGGB);
//	dcCamera.setup(0);
//		
//	w = dcCamera.getWidth();
//	h = dcCamera.getHeight();
//		
//	dcCamera.setShutterNorm(0.9);
//	dcCamera.setGainNorm(0.2);
//		
//	color.allocate(w, h);
//	grayscale.allocate(w, h);
//	
//	//640 480 - is the U and V values - not actually camera dimensions. 
//	dc1394_feature_whitebalance_set_value(dcCamera.getLibdcCamera(), 640, 480);
//	
//	setupMode();

}

//------------------------------------------------------------------------------------------------
void liveInput::setupRegularCamera( int width, int height, liveInputMode inputMode, int deviceId){
	
	inputType = LIVE_TYPE_CAM;
	this->inputMode = inputMode;
	
	camera.setDeviceID(deviceId);
	camera.initGrabber(width, height);
		
	w = camera.getWidth();
	h = camera.getHeight();
	
	setupMode();	
}

//------------------------------------------------------------------------------------------------
void liveInput::setupVideo(string videoPath, liveInputMode inputMode){
	
	inputType = LIVE_TYPE_VIDEO;
	this->inputMode = inputMode;

	videoPlayer.loadMovie(videoPath);
	videoPlayer.play();
	
	w = videoPlayer.getWidth();
	h = videoPlayer.getHeight();

	setupMode();		
}

//------------------------------------------------------------------------------------------------
bool liveInput::update(bool bFlipHoriz, bool bFlipVert, bool bRecord){
	bool bFrameNew = false;
	
	if( inputType == LIVE_TYPE_1394 ){

//		dcCamera.update();
//		if( dcCamera.isFrameNew() ){
//			color = dcCamera.getPixels();
//			color.mirror(bFlipVert, bFlipHoriz);
//			preview = color;
//			
//			bFrameNew = true;
//		}
		
	}
	else if( inputType == LIVE_TYPE_CAM ){
		camera.update();
		if( camera.isFrameNew() ){
			color = camera.getPixels();		
			color.mirror(bFlipVert, bFlipHoriz);

			preview = color;
//			grayscaleCur = preview;				
			bFrameNew = true;			
		}
	}else if( inputType == LIVE_TYPE_KINECT ){
		if( !ofGetKeyPressed('p') ) kinect.update();

		if( kinect.isFrameNew() ){
			unsigned char * pix = kinect.getDepthPixels();
			color2 = kinect.getPixels();
			color2.mirror(bFlipVert, bFlipHoriz);
			color2.setROI(0, 25, 610, 455);
			color.scaleIntoMe(color2, CV_INTER_LINEAR);
			color2.resetROI();

			grayscaleCur = pix;
			
			grayscaleCur.mirror(bFlipVert, bFlipHoriz);	
			grayscaleCur.setROI(0, 25, 610, 455);
			
			grayscalePre.scaleIntoMe(grayscaleCur, CV_INTER_LINEAR);
			grayscaleCur.resetROI();
			
			preview -= 180;			
			preview += grayscalePre;
					
//			preview.mirror(bFlipVert, bFlipHoriz);							

			bFrameNew = true;			
		}
	}
	else if( inputType == LIVE_TYPE_VIDEO ){
	
		if( ofGetKeyPressed('p') ){
			videoPlayer.setSpeed(0.001);
		}else if( ofGetKeyPressed('g') ){
			videoPlayer.setSpeed(1.0);
		}else if( ofGetKeyPressed('l') ){
			videoPlayer.setSpeed(0.1);
		}
	
		videoPlayer.update();
		bFrameNew = videoPlayer.isFrameNew();
		
		if( videoPlayer.getSpeed() < 0.1 ){
			bFrameNew = true;
		}
		
		if( bFrameNew ){
			color = videoPlayer.getPixels();
			color.mirror(bFlipVert, bFlipHoriz);	
			
			preview = color;			
//			grayscaleCur = preview;				
			bFrameNew = true;			
		}
		
		
	}else if( inputType == LIVE_TYPE_KINECT_SEQ ){
		seq.update();
		if( ofGetKeyPressed('p') ){
			seq.stop();
		}else{
			seq.play(); 
		}
		ofImage * frame = seq.getCurrentFrame();
		if( frame != NULL ){						
			unsigned char * rgba = frame->getPixels();
		
			unsigned char * colorPix = color.getPixels();
			unsigned char * alpha = preview.getPixels();
			
			int n = w*h*4;
			int a = 0;
			int rgb = 0;
			
			for(int i = 0; i < n; i+=4){
				colorPix[rgb]	= rgba[i];
				colorPix[rgb+1] = rgba[i+1];
				colorPix[rgb+2] = rgba[i+2];
				alpha[a]		= rgba[i+3];
				
				rgb += 3;
				a++;
			}
			
			color = colorPix;
			preview = alpha;
			
			color.mirror(bFlipVert, bFlipHoriz);
			preview.mirror(bFlipVert, bFlipHoriz);
			
			bFrameNew = true;						
		}
	}
	
	if( bFrameNew ){
//		cout << grayscaleCur.getWidth();
		grayscaleCur = preview;
	}
	
	if( bFrameNew ){
	
		if( bRecord ){
			
			if( preview.getWidth() > 0 ){
				if( recImg.getWidth() == 0 ){
					recImg.allocate(preview.getWidth(), preview.getHeight(), OF_IMAGE_GRAYSCALE);
				}
				
				if( sessionStr == "" ){
					sessionStr = "recordings/rec-"+ofGetTimestampString() + "/";
					
					if( !ofDirectory::doesDirectoryExist(sessionStr) ){
						ofDirectory::createDirectory( sessionStr, true, true);
					}
				}				
				recImg.setFromPixels(preview.getPixels(), preview.getWidth(), preview.getHeight(), OF_IMAGE_GRAYSCALE);
				recImg.saveImage(sessionStr + "f-"+ofToString(1000000+ofGetFrameNum()) + ".jpg", OF_IMAGE_QUALITY_BEST);
			}
		}else{
			sessionStr = "";
		}
	
		if( inputMode == LIVE_MODE_DIFF ){			

			if( inputType != LIVE_TYPE_KINECT ){
				grayscalePre	= grayscaleCur;
				grayscale.absDiff(grayscalePre, grayscaleCur);
				grayscale.threshold(threshold, false);

			}else{
					unsigned char * pix = grayscaleCur.getPixels();
					int numPixels = grayscaleCur.getWidth() * grayscaleCur.getHeight();

					int x = 0;
					int y = 0;
					for(int i = 0; i < numPixels; i++){
						if( x >= w ){
							x = 0;
							y++;
						}
						if( pix[i] < nearThreshold && pix[i] > farThreshold && y > minMaskY && y < maxMaskY ){
							pix[i] = 255;
						}else{
							pix[i] = 0;
						}
						x++;
					}
					
					grayscaleCur = pix;
					grayscale.absDiff(grayscalePre, grayscaleCur);
					grayscalePre = pix;
					
		//			grayscalePre	= grayscaleCur;
		//			grayscalePre.threshold(nearThreshold, true);
		//			grayscaleCur.threshold(farThreshold);
		//					
		//			cvAnd(grayscalePre.getCvImage(), grayscaleCur.getCvImage(), grayscale.getCvImage(), NULL);
		//			grayscale.flagImageChanged();
					
					//grayscale = grayscalePre;
					
			}
		}
		else if( inputMode == LIVE_MODE_KINECT_DEPTH ){			

			if( inputType == LIVE_TYPE_KINECT || inputType == LIVE_TYPE_VIDEO ){

//					unsigned char * pix = grayscaleCur.getPixels();
//					int numPixels = grayscaleCur.getWidth() * grayscaleCur.getHeight();
					
					grayscaleCur.threshold(3, false);
					
					grayscaleCur.setROI(ofRectangle(0, 0, 630, 480));

					extra1.scaleIntoMe(grayscaleCur, CV_INTER_LINEAR);	
					grayscaleCur.resetROI();
					extra1.blur(17);

//					extra2.scaleIntoMe(extra1, CV_INTER_LINEAR);
//					extra2.blur(9);
//					
//																														
//					int x = 0;
//					int y = 0;
//					for(int i = 0; i < numPixels; i++){
//						if( x >= w ){
//							x = 0;
//							y++;
//						}
//						if( pix[i] < nearThreshold && pix[i] > farThreshold && y > minMaskY && y < maxMaskY && x > minMaskX &&  x < maxMaskX ){
//							if( pix[i] - blurPix[i] >= threshold ){
//								pix[i] = ofMap(pix[i] - blurPix[i] , threshold, threshold * 2, 140, 255, true) ;
//							}else{
//								pix[i] = 0;
//							}
//						}else{
//							pix[i] = 0;
//						}
//						x++;
//					}
//					
//					grayscale = pix;
//					grayscale.blur(7);
//					grayscale.threshold(128, false);
										
			}
			
		}else if( inputMode == LIVE_MODE_BG_SUB ){			

			if( inputType != LIVE_TYPE_KINECT ){
//				grayscaleCur	= preview;
								
				if( bgMode == BG_MODE_ABS ){
					grayscale.absDiff(grayscalePre, grayscaleCur);
				}else if( bgMode == BG_MODE_LIGHTER ){
					grayscale = grayscaleCur;				
					grayscale -= grayscalePre;
				}else if( bgMode == BG_MODE_DARKER ){
					grayscale = grayscalePre;
					grayscale -= grayscaleCur;				
				}
				grayscale.threshold(threshold, false);

			}else{
										
					float * fp = bgFloat.getPixelsAsFloats();
					float * lv = liveFloat.getPixelsAsFloats();
																
					unsigned char * pix = grayscaleCur.getPixels();
					int numPixels = grayscaleCur.getWidth() * grayscaleCur.getHeight();

					int x = 0;
					int y = 0;
					
					float curPix;
					unsigned char curBg;
					
					for(int i = 0; i < numPixels; i++){
						if( x >= w ){
							x = 0;
							y++;
						}
						
//						curPix = (float)pix[i];
//						
//						lv[i] *= 0.7;
//						lv[i] += curPix * 0.3;
						
						curPix = (float)pix[i];
														
						if( fp[i] > curPix && curPix > 0 ){
							fp[i] *= 0.99;
							fp[i] += curPix * 0.01;
						}else if( curPix > 0 ){
							fp[i] *= 0.9996;
							fp[i] += curPix * 0.0004;						
						}						
						
						if( fp[i] < 0.01 ){
							fp[i] = 0.0;
						}
												
						curBg = fp[i];
						
						if( curPix < nearThreshold && curPix > farThreshold && y > minMaskY && y < maxMaskY ){
							if( curPix - curBg > threshold ){
								pix[i] = ofMap(curPix - curBg, threshold, threshold * 2, 140, 255, true) ;
							}							
							else pix[i] = 0;
						}else{
							pix[i] = 0;
						}
						x++;
					}
					
					bgFloat.flagImageChanged();
					
					grayscaleCur = pix;
					grayscaleCur.blur(7);
					grayscale = grayscaleCur;
					grayscale.threshold(128, false);
					grayscale.erode();
					grayscale.erode();
			}
		}
	}
	
	return 	bFrameNew;

}

		


