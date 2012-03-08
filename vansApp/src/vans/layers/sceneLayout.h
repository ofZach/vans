#pragma once
#include "baseLayer.h"
#include "positionableGraphic.h"
#include "genericSequence.h"
#include "ofxXmlSettings.h"

class animatedGraphic : public positionableGraphic{
	public:
	
		animatedGraphic(){
			bAnimate = false;
			bSequence = false;
			bVisible = true;
			color.set(255, 255, 255);
		}
		
		void setupSequence(string path, ofPoint _pos, ofPoint anchor, float scale){
			bSequence = true;
			cout << " loading seq from path " << path << endl; 
			seq.loadSequence(path, _pos, 20.0, anchor, scale);
			seq.play();
		}
	
		void draw(){	
			if( bVisible == false ) return;
			
			if( bAnimate )update(1.0);
			if( !bSequence ){
				positionableGraphic::draw();
			}else{
				seq.update();
				seq.draw();
			}
		}
		
		void update(float speedComp){
			float distX = fabs(maxX-minX);
			float distY = fabs(maxY-minY);
			
			float xComp = 25.0 / distX;
			float yComp = 25.0 / distY;
			
			pos.x = ofMap(ofSignedNoise(ofGetElapsedTimef() * rateX * xComp, unique), -1, 1, minX, maxX);
			pos.y = ofMap(ofSignedNoise(ofGetElapsedTimef() * rateY * yComp, -unique), -1, 1, minY, maxY);
		
			//cout << "animated pos is " << pos << "rateX is " << rateX << " rateY is " << rateY << " xcomp / ycomp " << xComp << " " << yComp << endl; 
		}
		
		bool bSequence;
		genericSequence seq;
		bool bVisible;
		ofPoint color;
		float unique;
		bool bAnimate;
		float minX, maxX;
		float minY, maxY;
		float rateX, rateY;
};

class sceneLayout{
	
	public:
		sceneLayout(){
			sceneBrightness = 1.0;
			shiftRange = 0.0;
			rotationRange = 0.0;
		}
		
		void setupScreenDimensions(float w, float h){
			screenW = w; 
			screenH = h;
		}
		
		vector <float> commaSepToFloat(string str){
			vector <string> res = ofSplitString(str, ",", true, true);
			vector <float> results; 
			for(int i = 0; i < res.size(); i++){
				results.push_back(atof(res[i].c_str())); 
			}
			return results;
		}
		
		void loadFromXml(string sceneXml){
			ofxXmlSettings xml;
			xml.loadFile(sceneXml);
			if(xml.pushTag("scene")){
				
				string folder = ofFilePath::getEnclosingDirectory(sceneXml);//xml.getValue("folderName","null"); 
				rotationRange = xml.getValue("rotationRange", 0.0);
				shiftRange = xml.getValue("shiftRange", 0.0);
				
				for(int j = 0; j < 3; j++){
					
					int num = 0;
					bool bMakeContours = false;
					
					vector <animatedGraphic> * graphicsPtr; 
					
					if( j == 0 ){
						num = xml.getNumTags("graphic");
						graphics.assign(num, animatedGraphic());
						graphicsPtr = &graphics;
					}
					
					if( j == 1 ){
						if( xml.tagExists("midground") ){
							xml.pushTag("midground");
							num = xml.getNumTags("graphic");
							graphicsMg.assign(num, animatedGraphic());
							graphicsPtr = &graphicsMg;
							cout<< " we are loading midground " << endl; 
						}else{
							continue;
						}
					}
					
					if( j == 2  ){
						if( xml.tagExists("foreground") ){
							xml.pushTag("foreground");
							num = xml.getNumTags("graphic");
							graphicsFg.assign(num, animatedGraphic());
							graphicsPtr = &graphicsFg;
							cout<< " we are loading foreground " << endl; 
						}else{
							continue;
						}
					}
				
					cout << " num graphics is " << num << endl; 
					
					for(int i = 0; i < num; i++){
						if( xml.pushTag("graphic", i) ){
							
							cout << xml.getValue("fileName","null") << endl; 
							cout << xml.getValue("pos",  "0.0,0.0,0.0") << endl; 
							cout << xml.getValue("anchor",  "0.0,0.0") << endl; 
							
							vector <float> pos = commaSepToFloat(xml.getValue("pos",  "0.0,0.0,0.0"));						
							vector <float> anchor = commaSepToFloat(xml.getValue("anchor", "0.0,0.0"));
							vector <float> rot = commaSepToFloat(xml.getValue("rot", "0.0,0.0,0.0,0.0"));
							vector <float> scale = commaSepToFloat(xml.getValue("scale", "1.0,1.0,1.0"));
		
							string filename = folder + xml.getValue("fileName","null");
							
							if( filename.length() && filename[filename.length()-1] == '/' ){
								graphicsPtr->at(i).setupSequence(filename, ofPoint(pos[0], pos[1], pos[2]), ofPoint(anchor[0], anchor[1]), scale[0]);
							}else{
								graphicsPtr->at(i).img.loadImage(filename, false, bMakeContours);
							}
														
							if( anchor.size() == 2 ){
								cout << " anchor is set! " << anchor[0] << " " << anchor[1] << endl; 
								graphicsPtr->at(i).img.setAnchorPercent(anchor[0], anchor[1]);
							}
							if( pos.size() == 3 ){
								cout << " pos is set! " << pos[0] << " " << pos[1] << " " << pos[2] << endl; 
								graphicsPtr->at(i).pos.set(pos[0], pos[1], pos[2]);
							}		
							if( rot.size() == 4 ){
								graphicsPtr->at(i).rotation.set(rot[0], rot[1], rot[2], rot[3]);
							}		
							if( scale.size() == 3 ){
								graphicsPtr->at(i).scale = scale[0];
							}	
							
							if( xml.pushTag("animate") ){
								
								graphicsPtr->at(i).minX = xml.getValue("minX", 0.0);
								graphicsPtr->at(i).maxX = xml.getValue("maxX", 0.0);
								graphicsPtr->at(i).minY = xml.getValue("minY", 0.0);
								graphicsPtr->at(i).maxY = xml.getValue("maxY", 0.0);
								graphicsPtr->at(i).unique = ofRandom(-1000, 1000);
								graphicsPtr->at(i).rateX = xml.getValue("rateX", 0.0);
								graphicsPtr->at(i).rateY = xml.getValue("rateY", 0.0);
								graphicsPtr->at(i).bAnimate = true;
								
								xml.popTag();
							}
							
							xml.popTag();
						}
					}
			
					if( j > 0 ){
						xml.popTag();
					}
				
				}
				
				xml.popTag();
			}
		}


		void update(){
			
		}
		
		void draw(){
			//glDisable(GL_DEPTH_TEST);
			for(int i = 0; i < graphics.size(); i++){
				ofSetColor(graphics[i].color.x * sceneBrightness, graphics[i].color.y * sceneBrightness, graphics[i].color.z * sceneBrightness);			
				graphics[i].draw(); 
			}
			//glEnable(GL_DEPTH_TEST);
		}

		void drawForeground(){
			//glDisable(GL_DEPTH_TEST);
			for(int i = 0; i < graphicsFg.size(); i++){
				ofSetColor(graphicsFg[i].color.x * sceneBrightness, graphicsFg[i].color.y * sceneBrightness, graphicsFg[i].color.z * sceneBrightness);			
				graphicsFg[i].draw(); 
			}
			//glEnable(GL_DEPTH_TEST);
		}

		void drawMidground(){
			//glDisable(GL_DEPTH_TEST);
			for(int i = 0; i < graphicsMg.size(); i++){
				ofSetColor(graphicsMg[i].color.x * sceneBrightness, graphicsMg[i].color.y * sceneBrightness, graphicsMg[i].color.z * sceneBrightness);
				graphicsMg[i].draw(); 
			}
			//glEnable(GL_DEPTH_TEST);
		}
	
		float rotationRange;
		float shiftRange;
		float sceneBrightness; 
		float screenW;
		float screenH;
		vector <animatedGraphic> graphics; 
		vector <animatedGraphic> graphicsFg; 
		vector <animatedGraphic> graphicsMg; 
		
		
};