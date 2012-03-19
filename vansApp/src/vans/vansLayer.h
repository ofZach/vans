#pragma once
#include "renderLayer.h"
#include "sceneLayout.h"
#include "graphicParticle.h"
#include "textureTrail.h"
#include "textTrail.h"
#include "seqParticle.h"


typedef struct {
    ofPoint pos;
    ofColor color;
    float alpha;
    float radius;
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
		
		vector <advImageSequence> graphics;
		vector <advImageSequence> graphicsAccents;
		vector <advImageSequence> graphicsSeqs;
		
		vector <seqParticle> pTests;
		vector <seqParticle> pTestsBack;
		
		std::map <int, textTrail> textTrails;
		std::map <int, textureTrail> trails;
    
        ofPoint midPtSmoothed;
        float presenceSmoothed;
    
		vector <textureTrail> trailDebug;
        
        vector < eventMarker > events;
        void debugDraw();
        
};