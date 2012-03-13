#pragma once
#include "ofMain.h"

class textureTrail{
	public:
	
	textureTrail(){
		imgPtr = NULL;
	}
	
	void setImage( ofImage * img ){
		imgPtr = img;
	}
	
	void setPolyLine( ofPolyline l ){
		
		line = l.getResampledBySpacing(3);
				
		lastSeen = ofGetElapsedTimef();
		makeMesh();
	}
	
	void makeMesh(){
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		
		for(int i = 0; i < line.size(); i++){
		
			float pct = ofMap(i, line.size()-1, 0, 1, true);
		
			ofPoint pre, nex;
			
			if( i == 0 ){
				pre = line[0];
				nex = line[1];
			}else if( i == line.size()-1 ){
				pre = line[i-1];
				nex = line[i];			
			}else{
				pre = line[i-1];
				nex = line[i+1];			
			}
			
			ofPoint d = nex-pre;
			ofPoint n(-d.y, d.x);
			
			mesh.addTexCoord(imgPtr->getTextureReference().getCoordFromPercent(pct, 0));
			mesh.addVertex(line[i] + n);
			mesh.addTexCoord(imgPtr->getTextureReference().getCoordFromPercent(pct, 1));
			mesh.addVertex(line[i] - n);
		}
		
		mesh.setupIndicesAuto();
	
	}
	
	void draw(){
		ofPushStyle();
		
			float alpha = ofMap(ofGetElapsedTimef() - lastSeen, 0.6, 2.0, 255, 0);
		
			ofSetColor(255, 255, 255, alpha);
			imgPtr->bind();
				mesh.drawFaces();
			imgPtr->unbind();
		
		ofPopStyle();
	}
	
	ofImage * imgPtr;
	float lastSeen;
	ofPolyline line;
	ofMesh mesh;
};