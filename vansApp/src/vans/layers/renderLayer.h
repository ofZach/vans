#pragma once
#include "ofMain.h"
#include "sceneLayer.h"
#include "sceneLayout.h"

static void setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float xShift = 0.0, float nearDist = 0, float farDist = 0) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();

	float eyeX = xShift + viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearDist, farDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);

	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	if(ofDoesHWOrientation()){
		if(vFlip){
			glScalef(1, -1, 1);
			glTranslatef(0, -height, 0);
		}
	}else{
		if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
		switch(orientation) {
			case OF_ORIENTATION_180:
				glRotatef(-180, 0, 0, 1);
				if(vFlip){
					glScalef(1, -1, 1);
					glTranslatef(-width, 0, 0);
				}else{
					glTranslatef(-width, -height, 0);
				}

				break;

			case OF_ORIENTATION_90_RIGHT:
				glRotatef(-90, 0, 0, 1);
				if(vFlip){
					glScalef(-1, 1, 1);
				}else{
					glScalef(-1, -1, 1);
					glTranslatef(0, -height, 0);
				}
				break;

			case OF_ORIENTATION_90_LEFT:
				glRotatef(90, 0, 0, 1);
				if(vFlip){
					glScalef(-1, 1, 1);
					glTranslatef(-width, -height, 0);
				}else{
					glScalef(-1, -1, 1);
					glTranslatef(-width, 0, 0);
				}
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				if(vFlip){
					glScalef(1, -1, 1);
					glTranslatef(0, -height, 0);
				}
				break;
		}
	}

}


class renderLayer : public sceneLayer{
	public:
	
		renderLayer(){
			rotAngle = 0.0;
			xShift = 0.0;
			curScene = NULL;			
		}
	
		virtual void setup( vector <ofFbo *> fboPtr ) = 0;
		virtual void loadSceneLayout(sceneLayout & layout){
			curScene = &layout;
		}
		
		virtual void renderScene(int personBrightness = 255){
			availableFbos[0]->begin();
		
				//ofSetupScreenPerspective(screenW, screenH, OF_ORIENTATION_DEFAULT, false, 55);
				
				if( trackerMan != NULL  ){
					
					if( trackerMan->finder.blobs.size() ){
						xShift = ofLerp(xShift, ofMap( trackerMan->finder.blobs[0].centroid.x, 150, 330, -curScene->shiftRange, curScene->shiftRange), 0.08); 
						rotAngle = ofLerp(rotAngle, ofMap( trackerMan->finder.blobs[0].centroid.x, 150, 330, -curScene->rotationRange, curScene->rotationRange), 0.08); 
					}else{
						rotAngle = ofLerp(rotAngle, 0.0, 0.08);
						xShift = ofLerp(xShift, 0.0, 0.08);
					}
					

					
				}
				
				setupScreenPerspective(screenW, screenH, OF_ORIENTATION_DEFAULT, false, 55, xShift);
				
				ofBackground(0, 0, 0, 255);
				
				
				ofPushMatrix(); 
				
					ofTranslate( ofGetWidth()/2, 0, 0 );
					ofRotate(-rotAngle, 0.0, 1.0, 0);
					ofTranslate( -ofGetWidth()/2, 0, 0 );
				
					curScene->draw();
				
				ofPopMatrix();
				
				if( trackerMan != NULL ){
					ofSetColor(personBrightness);
					trackerMan->rgbaPix.draw(0, 0, screenW, screenH);
				}
				
				ofPushMatrix(); 				
					ofTranslate( ofGetWidth()/2, 0, 0 );
					ofRotate(-rotAngle, 0.0, 1.0, 0);
					ofTranslate( -ofGetWidth()/2, 0, 0 );
					
					curScene->drawMidground();
				ofPopMatrix();

				curScene->drawForeground();
															
			availableFbos[0]->end();
			availableFbos[0]->readToPixels(curBuffPix);

		}
		
		ofPixels curBuffPix; 
		float rotAngle;
		float xShift;
		sceneLayout * curScene;
		vector <ofFbo *> availableFbos;
};