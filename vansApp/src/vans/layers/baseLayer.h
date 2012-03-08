
#pragma once
#include "ofMain.h"
#include "layerMessage.h"
#include "trackerManager.h"
#include "easySoundPlayer.h"
#include "ofxControlPanel.h"

extern ofxControlPanel * guiPtr; 
extern easySoundPlayer SP;

class baseLayer{
	public:
	
		baseLayer(){
			bRespondsToLightShader = true;
			amntofLight = 0.0;
			trackerMan = NULL;
		}
		
		virtual void setupScreenDimensions(float width, float height){
			screenW = width;
			screenH = height;
//			cout << " setup(float width, float height) not implemented " << endl;  
		}
		
		virtual void update(float speedComp = 1.0){
//			cout << " update(float speedComp = 1.0) not implemented " << endl;  
		}
		
		virtual void checkInteraction( trackerManager * tracker ){
			for(int k = 0; k < elements.size(); k++){
				elements[k]->checkInteraction(tracker);
			}
		}
		
		virtual void resetLight(){
			amntofLight = 0.0;
		}
		
		virtual void handleLightFade(){
			amntofLight *= 0.97;
			if( amntofLight < 0.01 ){
				amntofLight = 0.0;
			}
		}

		virtual void handleLight(){
			handleLight(0.043);
		}
		
		virtual void handleLight(float amnt, float cap = 1.5){
			amntofLight += amnt;
			if( amntofLight > cap){
				amntofLight = cap;
			}
		}
		
		virtual void drawIntoShader(){
		
		}
		
		virtual void draw() = 0;
		
		virtual void debugDraw(){
			if( ofGetKeyPressed('d') ){
				ofPushStyle();
					ofSetColor(255, 180, 90);
					ofCircle(pos.x, pos.y, 3.0);
									
					ofSetColor(255);
					ofDrawBitmapString("light: " + ofToString(amntofLight), pos.x, pos.y);
				ofPopStyle();
			}
		}

		virtual void addElement( vector <baseLayer *> & _elements ){
			for(int k = 0; k < _elements.size(); k++){
				elements.push_back( _elements[k] );
			}
		}	
		
		virtual void eventsIn(layerMessage & data){
			//cout << " eventsIn not implemented " << endl;  
		}
		
		virtual void addElement( baseLayer * _element ){
			elements.push_back( _element );
		}
		
		virtual vector <baseLayer *> & getElements(){
			return elements;
		}
		
		trackerManager * trackerMan;		
		
		bool bRespondsToLightShader;
		float amntofLight;
		
		ofPoint pos;
		vector <baseLayer *> elements;
		float screenW, screenH;
		
};