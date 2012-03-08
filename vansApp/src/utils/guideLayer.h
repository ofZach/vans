#pragma once
#include "baseLayer.h"
class guideLayer : public baseLayer{
	public:
	
	void loadGuide(string guideImagePath){
		guideImage.loadImage(guideImagePath);
		originalPath = guideImagePath;
	}
	
	void update(float speedPct){
	
	}
	
	vector <ofPoint> getCentroids(ofColor targetColor, float tolerance, ofPoint offset = 0){
		
		vector <ofPoint> pts;
		if( guideImage.type != OF_IMAGE_COLOR ){
			ofSystemAlertDialog("guide image needs to be rgb png - no transparency!");
			return pts;
		}	
		
		if( guideImage.width > 0 ){
		
			ofxCvGrayscaleImage binaryImg;
			binaryImg.allocate(guideImage.getWidth(), guideImage.getHeight());
			
			ofPixelsRef src = guideImage.getPixelsRef();
			ofPixelsRef dst = binaryImg.getPixelsRef();
			
			int numPix = 3 * guideImage.getWidth() * guideImage.getHeight();
			int d = 0;
			for(int k = 0; k < numPix; k+= 3){
				if( abs(src[k] - targetColor.r) < tolerance && abs(src[k+1] - targetColor.g) < tolerance && abs(src[k+2] - targetColor.b) < tolerance ){
					dst[d] = 255;
				}else{
					dst[d] = 0;
				}
				d++;
			}
			
			binaryImg.setFromPixels(dst);
			binaryImg.flagImageChanged();
			
			ofxCvContourFinder contour;
			contour.findContours(binaryImg, 4, 1920 * 1080, 100, false, true);
			
			for(int k = 0; k < contour.blobs.size(); k++){
				pts.push_back( contour.blobs[k].centroid  + offset);
			}
			
			//cout << "guide layer: num pts is " << pts.size() << endl; 
		
		} 	
		
		return pts;
	}
	
	vector <ofPolyline> getLines(ofColor targetColor, float tolerance, ofPoint offset = 0){
		vector <ofPolyline> lines;
		if( guideImage.type != OF_IMAGE_COLOR ){			
			ofSystemAlertDialog("guide image needs to be rgb png - no transparency!");
			return lines;
		}		

		if( guideImage.width > 0 ){
		
			ofxCvGrayscaleImage binaryImg;
			binaryImg.allocate(guideImage.getWidth(), guideImage.getHeight());
			
			ofPixelsRef src = guideImage.getPixelsRef();
			ofPixelsRef dst = binaryImg.getPixelsRef();
			
			int numPix = 3 * guideImage.getWidth() * guideImage.getHeight();
			int d = 0;
			for(int k = 0; k < numPix; k+= 3){
				if( abs(src[k] - targetColor.r) < tolerance && abs(src[k+1] - targetColor.g) < tolerance && abs(src[k+2] - targetColor.b) < tolerance ){
					dst[d] = 255;
				}else{
					dst[d] = 0;
				}
				d++;
			}
			
			binaryImg.setFromPixels(dst);
			binaryImg.flagImageChanged();
			
			ofxCvContourFinder contour;
			contour.findContours(binaryImg, 4, 1920 * 1080, 100, false, false);
			
			for(int k = 0; k < contour.blobs.size(); k++){
				vector <ofPoint> & p = contour.blobs[k].pts;

				ofPolyline poly;
				for(int i = 0; i < p.size(); i++){
					poly.addVertex(p[i] + offset);
				}
				lines.push_back( poly );
			}
			
			//cout << "guide layer: num lines is " << lines.size() << endl; 
		
		}	
		
		return lines;
	}	
	
	void draw(){
	
	}
	
	protected:

	
	string originalPath;
	ofImage guideImage;
};