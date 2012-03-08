/*
 *  ofxChromaKey.h
 *  openFrameworks
 *
 *  Created by theo on 09/09/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
#pragma once
 
#include "ofxOpenCv.h"
#include "ofMain.h"

class ofxChromaKey{

	public:
		void setup(int frameW, int frameH);
		
		void addKeyColorHSV( float hue, float sat, float val );
		void addKeyColorRGB( float r, float g, float b);
		
		void update( unsigned char * pixels );
		void update( ofBaseHasPixels * img );

		void draw(float x, float y);
	
				
		float s;
		float hueMax;
		float hueMin;
		float v;

		ofxCvColorImage colorImg;
		ofxCvColorImage hsvImg;
		ofxCvGrayscaleImage alphaImg;
		ofxCvGrayscaleImage cannyImg;
		ofxCvGrayscaleImage smallerImg;
		
		ofImage rgbaResult;
		
		int blurAmnt;
		
		int totalPixels;

		vector <ofColor> keyColors;
		
		protected:
		void process();

	
		unsigned char * alphaMask;
		
	protected:
		void recombineIntoRGBAImage();
};