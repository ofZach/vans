#include "vansLayer.h"
		
		
void ofPieSlice(float x, float y, float startAngle, float angleAmount, float radius){
	float numPts = ofMap( fabs(angleAmount), 0, 360, 3, 32, true);
	
	float sx = cos(startAngle*DEG_TO_RAD)*radius;
	float sy = sin(startAngle*DEG_TO_RAD)*radius;
		
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
}

		
//------------------------------------------------------------------------------------------------------------
void vansLayer::setup(vector <ofFbo *> fboPtr){
		
	availableFbos = fboPtr; 
	rotAngle = 0.0;
	
	ofDirectory dir;
	dir.listDir("graphicsBursts");
	
	graphics.assign(dir.size(),advImageSequence());
	for(int i = 0; i < dir.size(); i++){
		graphics[i].loadSeq(dir.getPath(i), "png");
	}

	dir.listDir("graphicsAccents");
	graphicsAccents.assign(dir.size(),advImageSequence());
	for(int i = 0; i < dir.size(); i++){
		graphicsAccents[i].loadSeq(dir.getPath(i), "png");
	}	

	dir.listDir("graphicsTextSeq");
	graphicsSeqs.assign(dir.size(),advImageSequence());
	for(int i = 0; i < dir.size(); i++){
		graphicsSeqs[i].loadSeq(dir.getPath(i), "png");
	}	
	
	trailImage.loadImage("graphicsTrails/swish01.png");
	
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

static bool killParticles( seqParticle & a ){
	return a.shouldKill();
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::checkInteraction( trackerManager * tracker ){
	trackerMan = tracker;
    	
	vector <trackBlob> feetBlobs = trackerMan->feetTracker.blobs;
    
    // [zach] this is accessing the blobs graph objects:
    
    for(int i = 0; i < feetBlobs.size(); i++){
		int id = feetBlobs[i].id;
		
		
        if( feetBlobs[i].graphs[1].getTriggered() == true ){
		
			if( feetBlobs[i].trail.size() && fabs(feetBlobs[i].speed.x) > 1.4 && trails[id].drawPct < 0.1 ){
				ofPolyline line; 
				
				ofPoint speed = feetBlobs[i].speed;
				speed.y *= 0.1;
				
				//make a curve for the trail to follow
				float curve		= ofRandom(0.1, 0.28);
				float len		= ofRandom(120, 340);
				float width		= ofRandom(1.2, 4.0);
				
				ofPoint d = (feetBlobs[i].cvBlob.centroid - feetBlobs[i].trail[0]).normalized();
				d *= len;
				
				ofPoint start	= feetBlobs[i].cvBlob.centroid - d*0.6;
				ofPoint dest	= feetBlobs[i].cvBlob.centroid + d*0.6;
				ofPoint mid		= (start + dest) / 2.0;
				ofPoint diff	= dest-start;
				ofPoint norm(-diff.y, diff.x);
				
				if( d.y < 0 ){
					ofPoint tmp = dest;
					dest = start;
					start = tmp;
				}
				
				line.addVertex(dest);
				line.bezierTo(mid + norm * curve, mid + norm * curve, start, 50);
				
				if( ofRandom(0, 100) > 50 ){
					//Ziiiiiiiiiiing
					textTrails[id].setImageSequence(&graphicsSeqs[0], ofRandom(25, 60));	
					textTrails[id].setPolyLine(line, width);
				}else{
					//graphic swish				
					trails[id].setImage(&trailImage);	
					trails[id].setPolyLine(line, width);				
				}
				
			}else{		
				
				//particles bursts 
				ofPoint speed = feetBlobs[i].speed;
				speed.y = fabs(speed.y) * -0.2;

				seqParticle p;
				seqParticle b;

				p.setup( feetBlobs[i].cvBlob.centroid, speed, ofRandom(0.3, 0.6) );
				p.setImageSequence( &graphics[ (int)ofRandom(0, (float)graphics.size() * 0.99) ] );

				b.setup( feetBlobs[i].cvBlob.centroid, speed * 0.7, ofRandom(0.3, 0.6) * 1.8 );
				b.setImageSequence( &graphicsAccents[ (int)ofRandom(0, (float)graphicsAccents.size() * 0.99) ] );

			
				pTests.push_back(p);
				pTestsBack.push_back(b); 
			}
        }
    }
	
	pTests.erase(pTests.begin(), partition(pTests.begin(), pTests.end(), killParticles));
	pTestsBack.erase(pTestsBack.begin(), partition(pTestsBack.begin(), pTestsBack.end(), killParticles));
    
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
		

	availableFbos[1]->begin();
			
		//comment out if you want to do acclum 
		ofClear(255, 255, 255, 255); 
	
		if( trackerMan != NULL ){
		
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
			
			std::map<int, textTrail>::iterator i = textTrails.begin();
			for( ; i != textTrails.end(); ++i ){
				i->second.draw();
				if( i->second.shouldKill() ){
					textTrails.erase(i);
				}
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
				
			std::map<int, textureTrail>::iterator itr = trails.begin();
			for( ; itr != trails.end(); ++itr ){
				itr->second.draw();
				if( itr->second.shouldKill() ){
					trails.erase(itr);
				}
			}
										
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

