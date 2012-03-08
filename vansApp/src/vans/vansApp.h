#pragma once

#include "baseLayer.h"
#include "sceneLayer.h"
#include "positionableGraphic.h"
#include "renderLayer.h"
#include "genericSequence.h"
#include "vansLayer.h"

class vansApp : public baseLayer{
	public:
		
		virtual void setupScreenDimensions(float width, float height);
			
		virtual void update(float speedComp = 1.0);
		virtual void checkInteraction( trackerManager * tracker );
		virtual void draw();	
		virtual void eventsIn(layerMessage & data);
					
		int currentScene;
		vansLayer vansMode0;
		
		vector<sceneLayout> layouts;
		vector <ofFbo *> availableFBOs;
		vector <renderLayer *> scenes;
	
};