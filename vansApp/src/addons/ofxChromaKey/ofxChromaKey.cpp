/*
 *  ofxChromaKey.cpp
 *  openFrameworks
 *
 *  Created by theo on 09/09/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxChromaKey.h"

void ofxChromaKey::setup(int frameW, int frameH){
	totalPixels = frameW * frameH;
	
	rgbaResult.setUseTexture(false);
	colorImg.setUseTexture(false);
	hsvImg.setUseTexture(false);
	alphaImg.setUseTexture(false);
	cannyImg.setUseTexture(false);
	smallerImg.setUseTexture(false);
	

	rgbaResult.allocate( frameW, frameH, OF_IMAGE_COLOR_ALPHA);
	colorImg.allocate(frameW, frameH);
	hsvImg.allocate(frameW/3, frameH/3);
	alphaImg.allocate(frameW, frameH);
	cannyImg.allocate(frameW, frameH);
	smallerImg.allocate(frameW/3, frameH/3);
	
	
	s			= 50;
	hueMax		= 99;
	hueMin		= 80;
	v			= 50;
	blurAmnt	= 5;
		
	alphaMask = new unsigned char[totalPixels];
}

void ofxChromaKey::addKeyColorHSV( float hue, float sat, float val ){
	keyColors.push_back( ofColor::fromHsb(hue, sat, val, 255.0f) );
}

void ofxChromaKey::addKeyColorRGB( float r, float g, float b){
	keyColors.push_back( ofColor(r, g, b) );
}


void ofxChromaKey::recombineIntoRGBAImage(){
	int total = totalPixels * 4;

	unsigned char * rgbPixels = colorImg.getPixels();
	unsigned char * rgbaPixels = rgbaResult.getPixels();
	unsigned char * alphaPix = alphaImg.getPixels();
	int rgb = 0;
	int alp = 0;
	
	for(int i = 0; i < total; i+= 4){
		
		rgbaPixels[i]   = rgbPixels[rgb];
		rgbaPixels[i+1] = rgbPixels[rgb+1];
		rgbaPixels[i+2] = rgbPixels[rgb+2];
		rgbaPixels[i+3] = alphaPix[alp];
		
		rgb += 3;
		alp ++;
	}
	
	rgbaResult.update();
	
}

void ofxChromaKey::update( unsigned char * pixels ){

	colorImg = pixels;
	process();

}

void ofxChromaKey::process(){	
	
	//printf("hsvImage.w is %i %i\n", hsvImg.width, hsvImg.height);

	hsvImg.scaleIntoMe(colorImg, CV_INTER_NN);
	hsvImg.convertRgbToHsv();
	
	unsigned char *	pix = hsvImg.getPixels();
	
	int totalColorPixels = hsvImg.width * hsvImg.height * 3;
	
	int k = 0;
	float pixHue;
	
	float hMin = ofMap(hueMin, 0, 255, 0, 180);
	float hMax = ofMap(hueMax, 0, 255, 0, 180);
	
	for(int i = 0; i < totalColorPixels; i+=3){
		 		
		//printf("pix is %i %i %i\n", pix[i], pix[i+1], pix[i+2]);
		
		if( pix[i+1] >= s  && pix[i+2] >= v){
		 
			pixHue = pix[i];
		 
			 //we do this to check the cases when the
			 //hue min could have wrapped
			 //or the hue max could have wrapped
			 //also if saturation is zero
			 //then hue doesn't matter hence (s == 0)               
			 if( (s == 0) || (pixHue >= hMin && pixHue <= hMax) ||
				(pixHue -180 >= hMin && pixHue -180 <= hMax) ||
				(pixHue +180 >= hMin && pixHue +180 <= hMax) ){
			   
				//we have a white pixel
				alphaMask[k] = 255;
			 }
			 else alphaMask[k] = 0;
		  }else alphaMask[k] = 0;
		
		//alphaMask[k] = 255;
		  k++;
	} 
	
	smallerImg = alphaMask;
	
	//smallerImg.scaleIntoMe(alphaImg, CV_INTER_LINEAR);
		
	smallerImg.invert();
//	smallerImg.blur(3);
//	smallerImg.threshold(190, false);
//	smallerImg.blur(3);
	
//	IplConvKernel * structure = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE, NULL);
//	cvMorphologyEx(smallerImg.getCvImage(), smallerImg.getCvImage(), NULL, structure, CV_MOP_CLOSE, 1);
//	cvReleaseStructuringElement(&structure);
	
	smallerImg.blur(blurAmnt);
	alphaImg.scaleIntoMe(smallerImg, CV_INTER_LINEAR);
	
	//cannyImg.flagImageChanged();
	
	//alphaImg = cannyImg;

//	alphaImg.erode_3x3();
//	alphaImg.dilate_3x3();

//	alphaImg.dilate_3x3();
//	alphaImg.erode_3x3();
//	alphaImg.erode_3x3();
//	alphaImg.erode_3x3();
//	alphaImg.dilate_3x3();
//	alphaImg.dilate_3x3();
//	alphaImg.dilate_3x3();
//	alphaImg.erode_3x3();
//	alphaImg.erode_3x3();

	//cvCanny(alphaImg.getCvImage(), cannyImg.getCvImage(), 10, 90);
	//cannyImg.flagImageChanged();

	//alphaImg.blurGaussian(11);
	
	//recombineIntoRGBAImage();
	

}

void ofxChromaKey::update( ofBaseHasPixels * img ){		

	//printf("colorImage is %i %i\n", colorImg.width, colorImg.height);

	

	unsigned char * pix = img->getPixels();
	update(pix);
}

void ofxChromaKey::draw(float x, float y){
	rgbaResult.draw(x, y);
}
