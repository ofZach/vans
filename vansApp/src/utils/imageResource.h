#pragma once

#include "ofImage.h"
#include "ofxOpenCv.h"

class imageResource{
	public:
		imageResource();
		
		void checkNeedsLoad();
		
		unsigned char * getPixels();
		ofPixelsRef getPixelsRef();	
		
		void setScale( float newScale ){
			scale = newScale;
		}
		
		float getWidth(){
			checkNeedsLoad();
			return width;
		}
		
		float getHeight(){
			checkNeedsLoad();
			return height;
		}
	
		bool loadImage( string filename, bool bDeferedLoad = false, bool bMakeContours = false );
		void setAnchorPercent(float xPct, float yPct);

		void draw(float x, float y, float z);
		void draw(float x, float y);
		void draw(float x, float y, float z, float w, float h);
		void draw(float x, float y, float w, float h);
		
		//TODO: - implement caching if drawing hasn't changed.
		vector <ofxCvBlob> getContours();
		
		string getImagePath(){
			return originalPath;
		}

		int debugColor;
		ofRectangle lastDraw;
		vector <ofxCvBlob> contours;
		ofPoint anchor;
		
		float width, height;

	protected:
		void saveXml( string xmlPath );
		void checkLoadXml( string xmlPath );
		void makeContoursFromAlpha();
	
		bool bNeedsLoad;
					
		float scale;
	
		bool debug;
		string originalPath;
		string key; 
		
		//ofxCvGrayscaleImage debugImg;
};