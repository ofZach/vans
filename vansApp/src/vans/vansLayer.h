#pragma once
#include "renderLayer.h"
#include "sceneLayout.h"
#include "graphicParticle.h"
#include "textureTrail.h"


typedef struct {
    ofPoint pos;
    ofColor color;
    float alpha;
} eventMarker;


class vansLayer : public renderLayer{

	public:
		virtual void setup(vector <ofFbo *> fboPtr);

		virtual void update(float speedComp = 1.0);
		virtual void checkInteraction( trackerManager * tracker );
		virtual void drawIntoShader();
		virtual void draw();
		void drawCirclesBackground();

		ofShader shaderFG, shaderBG;
		
		ofImage trailImage;
		vector <ofImage> graphics;
		vector <ofImage> graphicsAccents;

		vector <graphicParticle> pTests;
		vector <graphicParticle> pTestsBack;
    
    
        ofPoint midPtSmoothed;
        float presenceSmoothed;
    
		vector <textureTrail> trailDebug;
		std::map <int, textureTrail> trails;
    
        
        vector < eventMarker > events;
        void drawDebug();
    
    
};