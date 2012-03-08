#include "vansLayer.h"
		
//------------------------------------------------------------------------------------------------------------
void vansLayer::setup(vector <ofFbo *> fboPtr){
		
	availableFbos = fboPtr; 
	rotAngle = 0.0;
	
	bgPoints.assign(30, ofPoint());
	fgPoints.assign(30, ofPoint());
		
	for(int i = 0; i < bgPoints.size(); i++){
		bgPoints[i].set(ofRandom(0, screenW), ofRandom(0, screenH), ofRandom(8, 17));
	}

	for(int i = 0; i < fgPoints.size(); i++){
		fgPoints[i].set(ofRandom(0, screenW), ofRandom(0, screenH), ofRandom(14, 26));	
	}
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::update(float speedComp){

	float t = ofGetElapsedTimef();

	for(int i = 0; i < bgPoints.size(); i++){
		bgPoints[i].x += 1.9 * ofSignedNoise(t * 0.2, bgPoints[i].y * 0.08, i);
		bgPoints[i].y += 1.9 * ofSignedNoise(-t * 0.2, bgPoints[i].x * 0.09, -i);
		
		if( bgPoints[i].x > screenW ) bgPoints[i].x -= screenW;
		if( bgPoints[i].y > screenH ) bgPoints[i].y -= screenH;
		if( bgPoints[i].x < 0 ) bgPoints[i].x += screenW;
		if( bgPoints[i].y < 0 ) bgPoints[i].y += screenH;
	}

	for(int i = 0; i < fgPoints.size(); i++){
		fgPoints[i].x += 2.4 * ofSignedNoise(-t * 0.1, bgPoints[i].y * 0.05, i);
		fgPoints[i].y += 2.4 * ofSignedNoise(t * 0.1, bgPoints[i].x * 0.06, -i);
		
		if( fgPoints[i].x > screenW ) fgPoints[i].x -= screenW;
		if( fgPoints[i].y > screenH ) fgPoints[i].y -= screenH;
		if( fgPoints[i].x < 0 ) fgPoints[i].x += screenW;
		if( fgPoints[i].y < 0 ) fgPoints[i].y += screenH;

	}
	
	

//	if( ofGetFrameNum() % 30 == 0 ){
//		shader.load("", "shaders/paintedAlt.frag");
//		otherShader.load("", "shaders/paintAltMask.frag");
//		blendShader.load("", "shaders/blendLines.frag");	
//	}

}

//------------------------------------------------------------------------------------------------------------
void vansLayer::checkInteraction( trackerManager * tracker ){
	trackerMan = tracker;
	
	for(int k = 0; k < elements.size(); k++){
		elements[k]->checkInteraction(tracker);
	}
	
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::drawIntoShader(){
	
	//only neeeded if we have a scene
	if( curScene != NULL ){
		renderScene();
		
		//then we can get ourselves composited into the scene with ofPixels curBuffPix
	}
	
	availableFbos[1]->begin();
		
		//comment out if you want to do acclum 
		ofClear(255, 255, 255, 255); 
	
		if( trackerMan != NULL ){
				
			unsigned char * pix   = trackerMan->color.getPixels();
			unsigned char * alpha = trackerMan->alpha.getPixels();
		
			//stuff to play with - TODO add feet
			trackerMan->smoothedDepth; // depth of main blob 0-1 
			trackerMan->smoothedDiff; // amnt of movement 0-1
			//trackerMan->finder.blobs;  //blobs / contours

			ofSetColor( 200 );
			trackerMan->color.draw(0, 0, screenW, screenH);

			ofSetColor(100, 170, 255);
			for(int i = 0; i < bgPoints.size(); i++){
				ofCircle(bgPoints[i].x, bgPoints[i].y, bgPoints[i].z);
			}
			
			ofSetColor(255);
			//right now we just draw the rgba image in - nothing else
			//but we have access to seperate rgb and alpha pixels ( above ) and also contour 
			trackerMan->rgbaPix.draw(0, 0, screenW, screenH);

			ofSetColor(50, 230, 78);
			for(int i = 0; i < fgPoints.size(); i++){
				ofCircle(fgPoints[i].x, fgPoints[i].y, fgPoints[i].z);
			}		
			
			//lets draw some of the contour pts
			if( trackerMan->finder.blobs.size() ){				
				ofPushStyle();
					ofPushMatrix();
						ofSetColor(40, 40, 40);
						ofScale( screenW/trackerMan->color.getWidth(), screenH/trackerMan->color.getHeight(), 1);
						for(int k = 0; k < trackerMan->finder.blobs[0].pts.size(); k+= 10){
							ofCircle( trackerMan->finder.blobs[0].pts[k], 1);
						}
					ofPopMatrix();
				ofPopStyle();
			}
		}
																									
	availableFbos[1]->end();	
	
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::draw(){
				
	ofSetColor(255);
	availableFbos[1]->draw(0, 0, screenW, screenH);
			
	 
}

