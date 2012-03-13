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
	
	
	shaderFG.load("", "shaders/lines-fg.frag");
	shaderBG.load("", "shaders/lines-bg.frag");
    
    presenceSmoothed = 0;

}

//------------------------------------------------------------------------------------------------------------
void vansLayer::update(float speedComp){

	if( ofGetFrameNum() % 30 == 0 ){
		shaderFG.load("", "shaders/lines-fg.frag");
		shaderBG.load("", "shaders/lines-bg.frag");
	}

}

//------------------------------------------------------------------------------------------------------------
void vansLayer::checkInteraction( trackerManager * tracker ){
	trackerMan = tracker;
    
    
	
	vector <trackBlob> feetBlobs = trackerMan->feetTracker.blobs;
    
    // [zach] this is accessing the blobs graph objects:
    
    for(int i = 0; i < feetBlobs.size(); i++){
        if( feetBlobs[i].graphs[1].getTriggered() == true ){
            ofPoint speed = feetBlobs[i].speed;
			speed.y = fabs(speed.y) * -0.2;

            graphicParticle p;
            graphicParticle b;

			p.setup( feetBlobs[i].cvBlob.centroid, speed, ofRandom(0.3, 0.6) );
			p.setImage( &graphics[ (int)ofRandom(0, (float)graphics.size() * 0.99) ] );

			b.setup( feetBlobs[i].cvBlob.centroid, speed * 0.7, ofRandom(0.3, 0.6) * 1.8 );
			b.setImage( &graphicsAccents[ (int)ofRandom(0, (float)graphicsAccents.size() * 0.99) ] );

			
			pTests.push_back(p);
			pTestsBack.push_back(b); 
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

	for(int i = 0; i < pTestsBack.size(); i++){
		pTestsBack[i].update();
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
		
    
    ofPoint midPt;
    int count = 0;
    vector <trackBlob> feetBlobs = trackerMan->feetTracker.blobs;
    for(int i = 0; i < feetBlobs.size(); i++){
        midPt += feetBlobs[i].cvBlob.centroid;
        count++;
    }
    midPt /= (float)MAX(count, 1);
    midPt.x /= 640.0;
    midPt.y /= 480.0;
    
    if (count > 0){
        presenceSmoothed = 0.93f * presenceSmoothed + 0.07f * 1.0;
        midPtSmoothed = 0.91f * midPtSmoothed + 0.09f * midPt;
    } else {
        presenceSmoothed = 0.97f * presenceSmoothed + 0.03f * 0.0;
    }
    
    
	availableFbos[1]->begin();
			
		//comment out if you want to do acclum 
		ofClear(255, 255, 255, 255); 
	
		if( trackerMan != NULL ){
	
			ofPushMatrix(); 
			ofScale(screenW / trackerMan->getWidth(), screenH / trackerMan->getHeight(), 1);
						
			ofPixelsRef ref = trackerMan->color.getPixelsRef();
			ofPixelsRef alphaRef = trackerMan->alpha.getPixelsRef();
			
			//important - to update the pixels
			trackerMan->alpha.draw(0,0);
	
	
			//------ do background
			shaderBG.begin();
				shaderBG.setUniform1i("src_tex_unit0", 0);
				shaderBG.setUniform1i("src_tex_unit2", 2);
                shaderBG.setUniform1f("mx", midPtSmoothed.x);
                shaderBG.setUniform1f("my", midPtSmoothed.y);
                shaderBG.setUniform1f("amount", presenceSmoothed);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_RECTANGLE_ARB, trackerMan->alpha.getTextureReference().texData.textureID);				

				glActiveTexture(GL_TEXTURE0);
				ofSetColor(255);
				trackerMan->color.draw(0,0);
			shaderBG.end();
								
			ofSetColor(255);
			for(int i = 0; i < pTestsBack.size(); i++){
				pTestsBack[i].draw();
			}


			//------ do foreground
			shaderFG.begin();
				shaderFG.setUniform1i("src_tex_unit0", 0);
				shaderFG.setUniform1i("src_tex_unit2", 2);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_RECTANGLE_ARB, trackerMan->alpha.getTextureReference().texData.textureID);				

				glActiveTexture(GL_TEXTURE0);
				ofSetColor(255);
				trackerMan->color.draw(0,0);
			shaderFG.end();
					
			
			//drawCirclesBackground();			
			ofSetColor(255);
			for(int i = 0; i < pTests.size(); i++){
				pTests[i].draw();
			}
					
						
								
			ofPopMatrix();

		}
																									
	availableFbos[1]->end();	
	
}

void vansLayer::drawCirclesBackground(){

	bool bRandomPieStart = false;
	bool bInvert = ofGetKeyPressed('i');
	float spacing = 5.0;
	float pixelSize = 3.0;
	
	float defAngle = -90;
	
	if( bInvert ){
		ofSetColor(255, 255, 255);
	}else{
		ofSetColor(0, 0, 0);							
	}
		
	
	ofPixelsRef ref = trackerMan->color.getPixelsRef();
	ofPixelsRef alphaRef = trackerMan->alpha.getPixelsRef();
	
	int c = 0;
	int index =0;
	for(int y = 0; y < ref.getHeight(); y+= spacing ){
		c++;
		float offset = 0;
		if( c % 2 == 0 ) offset = spacing/2.0;
		
		for(int x = 0; x < ref.getWidth(); x+= spacing ){					
			float maskScale = 1.0;
			float scale = 1.0;
			
			int alphaIndex = y * 640 + x; 
			
			if( alphaRef[alphaIndex] > 10 )continue;
		
			ofColor col = ref.getColor(x + offset, y);
			float val = powf(col.getLightness()/255.0, 1.5);
			
			ofSetColor( col * 0.4 );
			
//					float angleAmount = ofMap(val, 1.0, 0.0, 0.0, 360.0, true);
//					if( bInvert ){
//						angleAmount = 360-angleAmount;
//					}
//					
//				
//					if( angleAmount > 0.0 ){
				if( bRandomPieStart ){
					defAngle = ofRandom(-90, 270);
				}
				ofCircle(offset + x, y, maskScale * scale * ofMap(val, 1.0, 0.0, pixelSize/8, pixelSize * 1.2, true));
				//ofPieSlice(offset + x, y, defAngle, angleAmount, maskScale * scale * ofMap(val, 1.0, 0.0, pixelSize/4, pixelSize/1.5, true));
//					}
		}
	}
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::draw(){
				
	ofSetColor(255);
	availableFbos[1]->draw(0, 0, screenW, screenH);
			
	 
}

