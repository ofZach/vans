#pragma once
#include "trackBlob.h"

class blobTracker{
	public:
	
		blobTracker(){
			w = h = 0;
			curId = 0;
		}
	
		void track(ofxCvContourFinder &finder, float matchDist){
		
			w = finder.getWidth();
			h = finder.getHeight();
			
			float idMatchDist = matchDist;	
		
			for(int k = 0; k < blobs.size(); k++){
				blobs[k].bMatched = false;
			}
			
			vector <trackBlob> tmpPts;
			
			for(int j = 0; j < finder.blobs.size(); j++){
				bool bFound = false;
						
				ofPoint newPt = finder.blobs[j].centroid;
				
				float smallestDist = 1000000.0; 
				int whichToMatch = -1;
				for(int k = 0; k < blobs.size(); k++){
					if( blobs[k].bMatched ) continue;

					float dist = (blobs[k].cvBlob.centroid - newPt).length();
					if( dist < smallestDist && dist < idMatchDist ){
						smallestDist = dist;
						whichToMatch = k;
						bFound = true;
					}
				}
				
				if( bFound ){
					int k = whichToMatch;
					
					blobs[k].speed *= 0.7;
					blobs[k].speed += (blobs[k].cvBlob.centroid - newPt) * 0.3;			
					blobs[k].dist = (blobs[k].cvBlob.centroid - newPt).length();
				
					blobs[k].keepAlive();							
					blobs[k].cloneFromBlob(finder.blobs[j]);
								
					tmpPts.push_back(blobs[k]);
					
					//cout << " GOOD MATCH " << endl <<endl;			
				}else{
					trackBlob tmp;
					tmp.birth(curId);				
					
					tmp.cloneFromBlob(finder.blobs[j]);
					
					tmp.dist  = 20;
					tmp.speed.set(0, 0, 0);
					tmpPts.push_back(tmp);
					
					//cout << " NO MATCH - ADDING FROM TMP " << curId << endl << endl;

					curId++;				
				}
				
				
				
			}	
						
			blobs.clear();
			blobs = tmpPts;	
			
			if( blobs.size() == 0 && curId > 10000000 ){
				curId = 0;
			}	
		}
		
		void draw(float x, float y, float _w, float _h){
			if( w == 0 ) return;
			
			ofPushStyle();
				ofPushMatrix();
					ofTranslate(x, y, 0);
					ofScale(_w/w, _h/h, 1);
					for(int i = 0; i < blobs.size(); i++){
						blobs[i].debugDraw();
					}
				ofPopMatrix();
			ofPopStyle();
            
            
            for (int i = 0; i < blobs.size(); i++){
                blobs[i].speedGraph.draw(x,y+ i * 34);
            }
            
            
		}
		
		int curId;
		float w,h;
		vector <trackBlob> blobs;
};