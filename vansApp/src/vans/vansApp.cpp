#include "vansApp.h"
#include "ofxControlPanel.h"

extern ofxControlPanel * guiPtr; 

void vansApp::setupScreenDimensions(float width, float height){
	
	trackerMan = NULL;
	screenW = width;
	screenH = height;
				
	availableFBOs.clear();
	availableFBOs.reserve(4);
	for(int i = 0; i < 4; i++){
		availableFBOs.push_back( new ofFbo());
		availableFBOs[i]->allocate(screenW, screenH, GL_RGBA, 0);	
	}
		
	//if we want to put the person into a 'world' we can make a sceneLayout and add it to the renderer
	//layouts.push_back(sceneLayout());
	//layouts[0].loadFromXml("sceneNew/0010-beginHouse/scene.xml");

	//setup one scene / mode 
	renderLayer * tmp = NULL;
	scenes.assign(1,  tmp); 
	scenes[0] = (&vansMode0);
	currentScene = 0;

	for(int i = 0; i < scenes.size(); i++){
		scenes[i]->setupScreenDimensions(screenW, screenH);
		scenes[i]->setup(availableFBOs);
		//scenes[i]->loadSceneLayout(layouts[i]);
	}	
		
}

void vansApp::eventsIn(layerMessage & data){
	
}

void vansApp::update(float speedComp){
		
	for(int k = 0; k < scenes.size(); k++){
		if( k == currentScene ){
			scenes[k]->update(speedComp);
		}
	}
		
}

void vansApp::checkInteraction( trackerManager * tracker ){
	
	trackerMan = tracker;
	
	for(int k = 0; k < elements.size(); k++){
		elements[k]->checkInteraction(tracker);
	}

	for(int k = 0; k < scenes.size(); k++){
		if( k == currentScene ){
			scenes[k]->checkInteraction(tracker);
		}
	}		
	
}

void vansApp::draw(){
	ofEnableAlphaBlending();
	ofSetColor(255);
	
	float drawH = ofGetHeight();
	float drawW = drawH * (screenW/screenH);

	//mostly here for old transform stuff - prob not needed
	ofPushView();
	ofPushStyle();
	ofPushMatrix();
			
	for(int k = 0; k < scenes.size(); k++){
		if( k == currentScene ){
			scenes[k]->drawIntoShader();
			scenes[k]->draw();			
		}
	}	
		
	if( guiPtr->getValueB("showTrackerDebug") ){
		if( trackerMan != NULL ){
			trackerMan->debugDraw();
		}
	}
			
	ofPopMatrix();
	ofPopStyle();
	ofPopView();

	ofEnableAlphaBlending();		
	
}
