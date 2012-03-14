#pragma once
#include "ofMain.h"

class textureTrail{
	public:
	
	textureTrail(){
		imgPtr = NULL;
		drawPct = 0.0;
		ribbonW = 3.0;
	}
	
	void setImage( ofImage * img ){
		imgPtr = img;
		drawPct = 0.0;		
	}
	
	void setPolyLine( ofPolyline l, float width ){
		line = l.getResampledBySpacing(3);
		drawPct = 0.1;
		lastSeen = ofGetElapsedTimef();
		makeMesh(drawPct);
		drawPct = 0.0;		
		ribbonW = width;
	}
	
	void makeMesh(float drawPct){
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		
		float scale = ofMap(drawPct, 0, 0.6, 0, 1, true);
		
		int numToDraw = (float)line.size() * drawPct;
		numToDraw = ofClamp(numToDraw, 0, line.size());
		
		int s =0;
		if( drawPct >= 1 ){
			s = ofMap( drawPct, 1, 1.99, 0, numToDraw-2, true);
		}
		
		if( numToDraw < 3 ) return; 
		
		for(int i = s; i < numToDraw; i++){
		
			float pct = ofMap(i, 0, line.size()-1, 0, 1, true);
			float dPct = MIN( ofMap(pct, 0, 0.1, 0.5, 1.0, true) , ofMap(pct, 0.9, 1.0, 1.0, 0.5, true) );
				
			ofPoint pre, nex;
			
			if( i == 0 ){
				pre = line[i];
				nex = line[i+1];
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
			mesh.addVertex(line[i] + n * ribbonW * dPct * scale );
			mesh.addTexCoord(imgPtr->getTextureReference().getCoordFromPercent(pct, 1));
			mesh.addVertex(line[i] - n * ribbonW * dPct * scale );
		}
		
		mesh.setupIndicesAuto();
	
	}
	
	void draw(){
		float alpha = ofMap(ofGetElapsedTimef() - lastSeen, 1.6, 2.3, 255, 0);
		if( alpha < 5 ) return;
		
		//makeMesh(1.0);
		cout << " drawing mesh " << drawPct << "with nPts: " << mesh.getNumVertices() << endl; 
		
		if( drawPct >= 0.0 && drawPct < 2.0 ){
			makeMesh(drawPct);
			drawPct += 0.08;
			
			if( drawPct > 2.0 ){
				drawPct = 0.0; 
			}
		}
	
		ofPushStyle();
		
			ofSetColor(255, 255, 255, alpha);
			imgPtr->bind();
				mesh.drawFaces();
			imgPtr->unbind();
		
		ofPopStyle();
	}
	
	float ribbonW;
	float drawPct;
	ofImage * imgPtr;
	float lastSeen;
	ofPolyline line;
	ofMesh mesh;
};