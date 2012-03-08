#pragma once

#include "baseLayer.h"
#include "imageResource.h"

class positionableGraphic : public baseLayer{
	public:
		
		ofPoint pos;
		ofVec4f rotation;
		float scale;
		
		positionableGraphic(){
			scale = 1.0;
		}
		
		void draw(){
			ofPushStyle();

//			ofEnableAlphaBlending();
//			ofSetColor(255);
			img.setScale(scale);
			if( rotation.w != 0.0 ){
				ofPushMatrix();
					ofTranslate(pos.x, pos.y, pos.z);		
					ofRotate(rotation.w, rotation.x, rotation.y, rotation.z);
					img.draw(0,0,0);
				ofPopMatrix();
			}else{
				img.draw(pos.x, pos.y, pos.z);
			}
			baseLayer::debugDraw();
			
			ofPopStyle();
		}

		vector <ofxCvBlob> getContours(){
			img.setScale(scale);
			img.draw(pos.x, pos.y);
			return img.getContours();
		}
		
		vector <ofPoint> getCenter(){
		
			img.setScale(scale);
			img.draw(pos.x, pos.y);
			vector <ofxCvBlob> contours = img.getContours();

			vector <ofPoint> centroids;
			
			for(int k = 0; k < contours.size(); k++){
				centroids.push_back( contours[k].centroid );
			}
			
			return centroids; 
			
		
//			cout << "POS IS " << pos << endl; 
//		
//			center.x = pos.x + img.getWidth() * scale * 0.5 -  img.getWidth() * scale * img.anchor.x;
//			center.y = pos.y + img.getHeight() * scale * 0.5 - img.getHeight() * scale * img.anchor.y;
//
//			cout << "CENTER IS " << center << endl; 

			
			//return center;
		
			//img.setScale(scale);
//			img.draw(pos.x, pos.y);
//
//			
//			vector <ofxCvBlob> contours = img.getContours();
//		
//			if( contours.size() && which < contours.size() ){
//				
//				for(int k = 0; k < contours[which].pts.size(); k++){
//					center += contours[which].pts[k];
//				}
//				center /= contours[which].pts.size();
//				
//				return center;
//			}
//			
//			return pos;
		}
		
		ofPoint center;
		imageResource img;

};