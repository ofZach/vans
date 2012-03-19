#pragma once
#include "ofMain.h"
#include "seqManager.h"

#define FONT_SPACING 1.4

class textTrail{
	public:
	
	textTrail(){
		drawPct = 0.0;
		alpha = 255;
		bInit = false;
	}
	
	void setImageSequence( advImageSequence * seqPtr, float fontH ){
		if( bInit ) return;
		drawPct = 0.0;	
		seq.setup(seqPtr, 30);
		numLetters = seq.seq->getNumImages();
		fontHeight = fontH;
		totalW = 0.0;
		letterW.clear();
		for(int i = 0; i < seq.seq->getNumImages(); i++){
			ofImage * img = seq.seq->getFrame(i);
			float w = fontHeight * ( img->getWidth() / img->getHeight() );
			letterW.push_back( w ); 
			totalW += w * FONT_SPACING;
		}
	}
		
	void setPolyLine( ofPolyline l, float width ){
		if( bInit ) return;		
		line = l.getResampledBySpacing(3);
		drawPct = 0.1;
		lastSeen = ofGetElapsedTimef();
		drawPct = 0.0;	
		alpha = 255;
		bInit = true;
		lastInit = ofGetElapsedTimef();					
	}
	
	virtual bool shouldKill(){
		return ( alpha <= 0 );
	}	
	
	void drawAlongLine(float drawPct){
		//drawPct = 1.0; 
	
		int numImages = seq.seq->getNumImages();
		if( numImages <= 0 )return; 
		
		int numToDraw	= (float)line.size() * drawPct;
		numToDraw		= ofClamp(numToDraw, 0, line.size());
		
		ofPolyline curLine = line;
		//curLine.resize(numToDraw);
		float lineLen = drawPct * 4.0 * (float) curLine.size();

		if( numToDraw < 3 ) return; 
		
		int	  letter = 0;
		bool  bDraw = false;
		
		vector <int> lettersToDraw; 
		float tmpDist = 0;
		int i = 0;
		int numF = 0;
		while( tmpDist < lineLen ){
			if( i >= numLetters ){ 
				break;
			}			
			lettersToDraw.push_back(i); 
			
			float nextDist = 0;
			
			if( i == letterW.size()-3 && tmpDist + (letterW[i+2] * FONT_SPACING) + (letterW[i+1] * FONT_SPACING) < lineLen * 0.9){
				numF++;
				//cout << " skipping " << i << endl; 
			}else{
				i++;
			}
			tmpDist += letterW[i] * FONT_SPACING;
			//cout << " tmpDist is " << tmpDist << " line len is " << lineLen <<endl;
		}
		//cout << " numToDraw " << numToDraw << endl; 
		//cout << " lettersToDraw is " << lettersToDraw.size() << " line.size() is " << line.size() << " line len is "  << lineLen <<  endl; 
		
		float dist = 0.0;
		float nextDist = 0.0; 
		int let = 0;
		
		for(int i = 0; i < drawPct * (float)curLine.size(); i++){
			
			ofPoint pre, nex;
			if( i == 0 ){
				pre = curLine[i];
				nex = curLine[i+1];
			}else if( i == curLine.size()-1 ){
				pre = curLine[i-1];
				nex = curLine[i];			
			}else{
				pre = curLine[i-1];
				nex = curLine[i+1];			
			}
						
			ofPoint d = nex-pre;
			ofPoint n(-d.y, d.x);
			
			float len = d.length();
			if( len == len ){
				dist += len;
			}
		
			//cout << " dist is " << dist << " nextDist is " << nextDist << " let is " << let << endl; 
						
			if( dist > nextDist && let < lettersToDraw.size() ){
			
				//cout << " drawing " << lettersToDraw[let] << endl; 
				ofImage * img = seq.seq->getFrame(lettersToDraw[let]);
				img->setAnchorPercent(0.5, 1.0);
				
				ofPushMatrix();
					ofTranslate(curLine[i]);
					ofRotate( atan2(d.y, d.x) * RAD_TO_DEG, 0, 0, 1);
					img->draw(0, 0, letterW[ lettersToDraw[let] ], fontHeight );
				ofPopMatrix();
				dist = 0;

				nextDist = 0.5 * letterW[ lettersToDraw[let] ] * FONT_SPACING;
				let++;
				if( let < lettersToDraw.size() ){
					nextDist += 0.5 * letterW[ lettersToDraw[let] ] * FONT_SPACING;
				}
			}
			
		}
	
	}
	
	void draw(){
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, alpha);
	
		alpha = ofMap(ofGetElapsedTimef() - lastSeen, 1.2, 1.9, 255, 0);
		if( alpha < 5 ) return;
				
		if( drawPct >= 0.0 && drawPct < 2.0 ){
			drawPct += 0.22;
			
			if( drawPct >= 2.0 ){
				drawPct = 2.0; 
			}
		}
	
		drawAlongLine(ofClamp(drawPct, 0, 1));
//		ofSetColor(255, 0, 0, alpha);
//		line.draw();
	
//		ofPushStyle();
//		
//			ofSetColor(255, 255, 255, alpha);
//			imgPtr->bind();
//				mesh.drawFaces();
//			imgPtr->unbind();
//		
//		ofPopStyle();

		ofPopStyle();
	}
	
	bool bInit;
	float lastInit;
	
	vector <float> letterW;
	seqManager seq;
	
	int numLetters;
	float totalW;
	float fontHeight;
	float texH;
	float lineLen;	
	float alpha;
	float drawPct;
	float lastSeen;
	ofPolyline line;
};