#pragma once
#include "renderLayer.h"
#include "sceneLayout.h"

class vansLayer : public renderLayer{

	public:
		virtual void setup(vector <ofFbo *> fboPtr);

		virtual void update(float speedComp = 1.0);
		virtual void checkInteraction( trackerManager * tracker );
		virtual void drawIntoShader();
		virtual void draw();

		vector <ofPoint> bgPoints;		
		vector <ofPoint> fgPoints;
};