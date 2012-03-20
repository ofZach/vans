#include "vansLayer.h"
		
		

bool noAlpha(eventMarker & m){
    return m.alpha < 0.01;
}

ofDirectory bass;
ofDirectory cymbals;
ofDirectory snareDrums;
ofDirectory percussion;
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
    
    
    bass.listDir("sounds/Bass");
    for (int i = 0; i < bass.size(); i++){
         ezsnd.addSound(bass.getPath(i), bass.getName(i), true);
    }
    
    cymbals.listDir("sounds/Cymbals");
    for (int i = 0; i < cymbals.size(); i++){
        ezsnd.addSound(cymbals.getPath(i), cymbals.getName(i), true);
    }
    
    snareDrums.listDir("sounds/Snaredrums");
    for (int i = 0; i < snareDrums.size(); i++){
        ezsnd.addSound(snareDrums.getPath(i), snareDrums.getName(i), true);
    }
    
    percussion.listDir("sounds/Percussion");
    for (int i = 0; i < percussion.size(); i++){
        ezsnd.addSound(percussion.getPath(i), percussion.getName(i), true);
    }
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
    
	float maxLen    = guiPtr->getValueF("maxDistFootMove");
	float scaleComp = guiPtr->getValueF("scaleComp");
	float crazyModeScaleAmnt = guiPtr->getValueF("crazyModeScaleAdd", false);

	float bonusScale = scaleComp * ofMap(tracker->normalizedSmoothedSpeed, 0.6, 1.0, 1.0, crazyModeScaleAmnt) * 1.2;
	
    // deal with events! 
    for(int i = 0; i < feetBlobs.size(); i++){
		int id		= feetBlobs[i].id;
		float lenPct = feetBlobs[i].getRoughDistPct(maxLen);
		
        for (int j = 0; j < 4; j++){
            if( feetBlobs[i].graphs[j].getTriggered() == true ){
                eventMarker marker;
                marker.pos = feetBlobs[i].cvBlob.centroid;
                
                float timePct = feetBlobs[i].timeSeen / 0.5f;
                if (timePct > 1) timePct = 1;
				
				float speedPct = ofClamp(lenPct*2.0, 0, 1);
				float speedPct2 = lenPct;//ofClamp((lenPct-0.5)* 2.0, 0, 1);
				
				cout << " speedPct is " << speedPct << endl; 
				cout << " speedPct2 is " << speedPct2 << endl; 
				
//                float speedPct = feetBlobs[i].speed.length() / 4.0;
//                if (speedPct > 1) speedPct = 1;
//                
//                float speedPct2 = feetBlobs[i].speed.length() / 8.0;
//                if (speedPct2 > 1) speedPct2 = 1;
                
                
                float vol = 0.8 * powf(timePct * speedPct, 2);
                
                float pan = ofMap(marker.pos.x, 0,640, -1,1);
                
                string name;
                int which;
                marker.radius = 50 * timePct * speedPct;
                switch (j){
                    case 0:
                        
                        which = (int)ofRandom(0,bass.size()) % bass.size();
                        name = bass.getName(which);
                        ezsnd.play(name, vol, ofRandom(0.8,1.0), pan);
                        marker.color = ofColor::red;
                        
                        if (speedPct > 0.95){
                            //particles bursts 
                            ofPoint speed = feetBlobs[i].speed;
                            speed.x *= -1;
                            speed.y = fabs(speed.y) * -0.2;
                            
                            seqParticle p;
                            seqParticle b;
                            
                            p.setup( feetBlobs[i].cvBlob.centroid, speed, ofRandom(0.3, 0.6)* speedPct2 * bonusScale , ofRandom(0.6, 1.0) );
                            p.setImageSequence( &graphics[ (int)ofRandom(0, (float)graphics.size() * 0.99) ] );
                            
                            b.setup( feetBlobs[i].cvBlob.centroid, speed * 0.7, ofRandom(0.3, 0.6) * 1.8 * speedPct2 * bonusScale );
                            b.setImageSequence( &graphicsAccents[ (int)ofRandom(0, (float)graphicsAccents.size() * 0.99) ] );
                            
                            pTests.push_back(p);
                            pTestsBack.push_back(b); 
                        }
                        
						//make crazy bg blobs 
                        if( tracker->normalizedSmoothedSpeed >= 0.9 ){
							if( ofRandom(0, 1000) > 900 ){
								for(int i = 0; i < 4; i++){
									seqParticle b;
									b.setup( ofPoint(ofRandom(100, screenW-100), ofRandom(100, screenH-400)),  ofPoint( ofRandom(-1,1),  ofRandom(-1,1) ) * 2.0 , ofRandom(0.3, 0.6) * 1.3 * 1.7 );
									b.setImageSequence( &graphicsAccents[ graphicsAccents.size() -1 ] );
									pTestsBack.push_back(b); 
								}
							}
						}
                        
                        
                        break;
                    case 1:
                        which = (int)ofRandom(0,cymbals.size()) % cymbals.size();
                        name = cymbals.getName(which);
                        ezsnd.play(name, vol, ofRandom(0.8,1.0), pan);
                        
                        marker.color = ofColor::cyan;
                        break;
                    case 2:
                        which = (int)ofRandom(0,percussion.size()) % percussion.size();
                        name = percussion.getName(which);
                        ezsnd.play(name, vol, ofRandom(0.8,1.0), pan);
                        marker.color = ofColor::blue;
                        
                        if (speedPct > 0.95){
                            //particles bursts 
                            ofPoint speed = feetBlobs[i].speed;
                            speed.x *= -1;
                            speed.y = fabs(speed.y) * -0.2;
                            
                            seqParticle p;
                            seqParticle b;
                            
                            p.setup( feetBlobs[i].cvBlob.centroid, speed, ofRandom(0.3, 0.6) * speedPct2  * bonusScale, ofRandom(0.5,  1.0));
                            p.setImageSequence( &graphics[ (int)ofRandom(0, (float)graphics.size() * 0.99) ] );
                            
                            b.setup( feetBlobs[i].cvBlob.centroid, speed * 0.7, ofRandom(0.3, 0.6) * 1.8  * speedPct2 * bonusScale);
                            b.setImageSequence( &graphicsAccents[ (int)ofRandom(0, (float)graphicsAccents.size() * 0.99) ] );
                            
                            pTests.push_back(p);
                            pTestsBack.push_back(b); 
                        }
                        
                        
                        break;
                    case 3: 
                        which = (int)ofRandom(0,snareDrums.size()) % snareDrums.size();
                        name = snareDrums.getName(which);
                        ezsnd.play(name, vol, ofRandom(0.8,1.0), pan);
                        marker.color = ofColor::yellow;
                        break;
                }
                marker.alpha = 1;
                events.push_back(marker);
            }
        }
    }
    
    
    ofRemove(events, noAlpha);
    
    for (int i = 0; i < events.size(); i++){
        events[i].alpha *= 0.91;
    }
    

    
    
    // [zach] this is accessing the blobs graph objects:
    
    for(int i = 0; i < feetBlobs.size(); i++){
		int id = feetBlobs[i].id;
		
		float lenPct= feetBlobs[i].getRoughDistPct(maxLen);
		
        if( feetBlobs[i].graphs[1].getTriggered() == true ){
		
			if( feetBlobs[i].trail.size() && fabs(feetBlobs[i].speed.x) > 1.4 && trails[id].drawPct < 0.1 ){
				ofPolyline line; 
				
				ofPoint speed = feetBlobs[i].speed;
				speed.y *= 0.1;
				
				//make a curve for the trail to follow
				float curve		= ofRandom(0.1, 0.28);
				float len		= ofRandom(120, 340) * (0.4 + lenPct);
				float width		= ofRandom(1.2, 2.5 + lenPct * 2.8);
				
				ofPoint b = feetBlobs[i].smoothTrail[0];
				ofPoint d = (feetBlobs[i].cvBlob.centroid - b).normalized();
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
					textTrails[id].setImageSequence(&graphicsSeqs[0], ofRandom(28, 65) * ( 0.6 + lenPct ) * bonusScale  );	
					textTrails[id].setPolyLine(line, width);
				}else{
					//graphic swish				
					trails[id].setImage(&trailImage);	
					trails[id].setPolyLine(line, width * bonusScale);				
				}
				
			}else{		
				
				
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
			
			std::map<int, textureTrail>::iterator itr = trails.begin();
			for( ; itr != trails.end(); ++itr ){
				itr->second.draw();
				if( itr->second.shouldKill() ){
					trails.erase(itr);
				}
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
				

										
			//drawCirclesBackground();			
			ofSetColor(255);
			for(int i = 0; i < pTests.size(); i++){
				pTests[i].draw();
			}

			ofPopMatrix();

		}
																									
	availableFbos[1]->end();	
	
}


//------------------------------------------------------------------------------------------------------------
void vansLayer::draw(){
				
	ofSetColor(255);
	availableFbos[1]->draw(0, 0, screenW, screenH);
			
	 
}

//------------------------------------------------------------------------------------------------------------
void vansLayer::debugDraw(){
    
    ofEnableAlphaBlending();
    ofFill();
    for (int i = 0; i < events.size(); i++){
        events[i].alpha *= 0.97;
        ofSetColor(events[i].color.r, events[i].color.g, events[i].color.b, events[i].alpha*255);
        ofCircle(ofMap(events[i].pos.x, 0,640,0,1024), ofMap(events[i].pos.y, 0,480,0,768), events[i].radius);
        
    }
    
    
}

