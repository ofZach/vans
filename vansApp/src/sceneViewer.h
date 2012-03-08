#pragma once
#include "ofMain.h"

class sceneViewer : public ofBaseDraws{
	public:
	
	void setup( string imgPath ){
		img.loadImage(imgPath);
	}
	
	void addDrawable( ofBaseDraws * ptr, ofPoint topLPt, ofPoint scale = 1.0, float alpha = 1.0 ){
		elements.push_back(ptr);
		posArr.push_back( topLPt );
		scaleAmnt.push_back( scale );
		alphaVal.push_back( alpha);
	}
	
	void draw(float x, float y){
		img.draw(x, y);
		
		for(int k = 0; k < elements.size(); k++){
			elements[k]->draw(posArr[k].x, posArr[k].y, elements[k]->getWidth() * scaleAmnt[k].x,  elements[k]->getHeight() * scaleAmnt[k].y );
		}	
	}
	
	void draw(float x, float y, float w, float h){
		float maxW = img.getWidth();
		float maxH = img.getHeight();
		
		ofPushStyle();
		ofDisableAlphaBlending();
		ofSetColor(255, 255, 255, 255);
	
		ofPushMatrix();
		ofTranslate(x, y, 0);
		ofScale( w/maxW, h/maxH, 1.0 );
	
		img.draw(0, 0);
		for(int k = 0; k < elements.size(); k++){
			ofSetColor(255, 255, 255, 255 );
			elements[k]->draw(posArr[k].x, posArr[k].y, elements[k]->getWidth() * scaleAmnt[k].x,  elements[k]->getHeight() * scaleAmnt[k].y );
		}	
		
		ofPopMatrix();
		ofPopStyle();
	}	
	
	float getWidth(){
		return img.getWidth();
	}
	
	float getHeight(){
		return img.getHeight();
	}	
	
	ofImage img;
	
	vector <ofBaseDraws *> elements;
	vector <ofPoint> posArr;
	vector< ofPoint> scaleAmnt;
	vector <float> alphaVal;

};