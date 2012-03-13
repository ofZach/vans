#include "vansLayer.h"
		
		
void ofPieSlice(float x, float y, float startAngle, float angleAmount, float radius){
	float numPts = ofMap( fabs(angleAmount), 0, 360, 3, 32, true);
	
	float sx = cos(startAngle*DEG_TO_RAD)*radius;
	float sy = sin(startAngle*DEG_TO_RAD)*radius;
	
	//float ex = cos((startAngle+angleAmount)*DEG_TO_RAD)*radius;
	//float ey = sin((startAngle+angleAmount)*DEG_TO_RAD)*radius;
	
	float delta = ( angleAmount*DEG_TO_RAD ) / (numPts-1.0);
	float angle = startAngle * DEG_TO_RAD;
	
	ofBeginShape();
	ofVertex(x, y);
	for(int k = 0; k < numPts; k++){
		float sx = cos(angle)*radius;
		float sy = sin(angle)*radius;
		ofVertex(x+sx, y+sy);	
		angle += delta;		
	}
	ofVertex(x, y);	
	ofEndShape(false);
	
//	float midX = (ex - sx) / 2;
//	float midY = (ey - sy) / 2;
//	
//	float length = sqrt( midX*midX + midY*midY );
//	
//	ofVec2f relStart(sx, sy);
//	ofVec2f relEnd(ex, ey);
//	
//	ofVec2f perpL = ( -relStart.y, relStart.x );
//	ofVec2f perpR = ( relEnd.y, -relEnd.x );
//	
//	perpL *= length / radius;
//	perpR *= length / radius;
		
//	ofBeginShape;
		//ofTriangle(x, y, x + sx, y + sy, x + ex, y + ey);
//		ofVertex(x + sx, y + sy);
//		ofVertex(x + ex, y + ey);
//		ofBezierVertex(perpL.x, perpL.y, perpR.x, perpR.y, ex, ey);
//	ofEndShape(true);
}
		
//------------------------------------------------------------------------------------------------------------
void vansLayer::setup(vector <ofFbo *> fboPtr){
		
	availableFbos = fboPtr; 
	rotAngle = 0.0;
	
	ofDirectory dir;
	dir.listDir("graphicsBursts");
	
	graphics.assign(dir.size(),ofImage());
	for(int i = 0; i < dir.size(); i++){
		graphics[i].loadImage(dir.getPath(i));
		graphics[i].setAnchorPercent(0.5, 0.5);
	}

	dir.listDir("graphicsAccents");
	graphicsAccents.assign(dir.size(),ofImage());
	for(int i = 0; i < dir.size(); i++){
		graphicsAccents[i].loadImage(dir.getPath(i));
		graphicsAccents[i].setAnchorPercent(0.5, 0.5);
	}	
	
	
	shader.load("", "shaders/woodCutOg.frag");

}

//------------------------------------------------------------------------------------------------------------
void vansLayer::update(float speedComp){

	if( ofGetFrameNum() % 30 == 0 ){
		shader.load("", "shaders/woodCutOg.frag");
	}

}

//------------------------------------------------------------------------------------------------------------
void vansLayer::checkInteraction( trackerManager * tracker ){
	trackerMan = tracker;
    
    
	
	vector <trackBlob> feetBlobs = trackerMan->feetTracker.blobs;
    
    // [zach] this is accessing the blobs graph objects:
    
    for(int i = 0; i < feetBlobs.size(); i++){
        if( feetBlobs[i].speedGraph.getTriggered() == true ){
            graphicParticle p;
            ofPoint speed = feetBlobs[i].speed;
			speed.y = fabs(speed.y) * -0.2;
			
			p.setup( feetBlobs[i].cvBlob.centroid, speed, ofRandom(0.3, 0.6) );
			p.setImage( &graphics[ (int)ofRandom(0, (float)graphics.size() * 0.99) ] );
			pTests.push_back(p);
        }
    }
    
    //old
	for(int i = 0; i < feetBlobs.size(); i++){
		if( feetBlobs[i].speed.y * feetBlobs[i].preSpeed.y < -0.3 ){

		}	
	}
	
	for(int i = 0; i < pTests.size(); i++){
		pTests[i].update();
	}
	
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
	
	bool bRandomPieStart = false;
	bool bInvert = ofGetKeyPressed('i');
	float spacing = 5.0;
	float pixelSize = 3.0;
	
	availableFbos[1]->begin();
			
		//comment out if you want to do acclum 
		ofClear(255, 255, 255, 255); 
	
		if( trackerMan != NULL ){
	
			ofPushMatrix(); 
			ofScale(screenW / trackerMan->getWidth(), screenH / trackerMan->getHeight(), 1);
						
			ofPixelsRef ref = trackerMan->color.getPixelsRef();
			ofPixelsRef alphaRef = trackerMan->alpha.getPixelsRef();
			
			//to update the pixels
			trackerMan->alpha.draw(0,0);
			
			//do background
			shader.begin();
				shader.setUniform1i("src_tex_unit0", 0);
				shader.setUniform1i("src_tex_unit2", 2);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_RECTANGLE_ARB, trackerMan->alpha.getTextureReference().texData.textureID);				

				glActiveTexture(GL_TEXTURE0);
				ofSetColor(255);
				trackerMan->color.draw(0,0);
			shader.end();
					
			
			//do foreground
			float defAngle = -90;
			
			if( bInvert ){
				ofSetColor(255, 255, 255);
			}else{
				ofSetColor(0, 0, 0);							
			}
			
//			int c = 0;
//			int index =0;
//			for(int y = 0; y < ref.getHeight(); y+= spacing ){
//				c++;
//				float offset = 0;
//				if( c % 2 == 0 ) offset = spacing/2.0;
//				
//				for(int x = 0; x < ref.getWidth(); x+= spacing ){					
//					float maskScale = 1.0;
//					float scale = 1.0;
//					
//					int alphaIndex = y * 640 + x; 
//					
//					if( alphaRef[alphaIndex] > 10 )continue;
//				
//					ofColor col = ref.getColor(x + offset, y);
//					float val = powf(col.getLightness()/255.0, 1.5);
//					
//					ofSetColor( col * 0.4 );
//					
////					float angleAmount = ofMap(val, 1.0, 0.0, 0.0, 360.0, true);
////					if( bInvert ){
////						angleAmount = 360-angleAmount;
////					}
////					
////				
////					if( angleAmount > 0.0 ){
//						if( bRandomPieStart ){
//							defAngle = ofRandom(-90, 270);
//						}
//						ofCircle(offset + x, y, maskScale * scale * ofMap(val, 1.0, 0.0, pixelSize/8, pixelSize * 1.2, true));
//						//ofPieSlice(offset + x, y, defAngle, angleAmount, maskScale * scale * ofMap(val, 1.0, 0.0, pixelSize/4, pixelSize/1.5, true));
////					}
//				}
//			}	
			
			ofSetColor(255);
			for(int i = 0; i < pTests.size(); i++){
				pTests[i].draw();
			}
						
//			unsigned char * alpha = trackerMan->alpha.getPixels();
//		
//			//stuff to play with - TODO add feet
//			trackerMan->smoothedDepth; // depth of main blob 0-1 
//			trackerMan->smoothedDiff; // amnt of movement 0-1
//			//trackerMan->finder.blobs;  //blobs / contours
//
//			ofSetColor( 200 );
//			trackerMan->color.draw(0, 0, screenW, screenH);
//
//			ofSetColor(100, 170, 255);
//			for(int i = 0; i < bgPoints.size(); i++){
//				ofCircle(bgPoints[i].x, bgPoints[i].y, bgPoints[i].z);
//			}
//			
//			ofSetColor(255);
//			//right now we just draw the rgba image in - nothing else
//			//but we have access to seperate rgb and alpha pixels ( above ) and also contour 
//			trackerMan->rgbaPix.draw(0, 0, screenW, screenH);
//
//			ofSetColor(50, 230, 78);
//			for(int i = 0; i < fgPoints.size(); i++){
//				ofCircle(fgPoints[i].x, fgPoints[i].y, fgPoints[i].z);
//			}		
//			
//			//lets draw some of the contour pts
//			if( trackerMan->finder.blobs.size() ){				
//				ofPushStyle();
//					ofPushMatrix();
//						ofSetColor(40, 40, 40);
//						ofScale( screenW/trackerMan->color.getWidth(), screenH/trackerMan->color.getHeight(), 1);
//						for(int k = 0; k < trackerMan->finder.blobs[0].pts.size(); k+= 10){
//							ofCircle( trackerMan->finder.blobs[0].pts[k], 1);
//						}
//					ofPopMatrix();
//				ofPopStyle();
//			}


			ofPopMatrix();

		}
																									
	availableFbos[1]->end();	
	
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::draw(){
				
	ofSetColor(255);
	availableFbos[1]->draw(0, 0, screenW, screenH);
			
	 
}

