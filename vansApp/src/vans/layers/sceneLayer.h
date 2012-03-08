#pragma once
#include "baseLayer.h"

class sceneLayer : public baseLayer{
	public:
	
		sceneLayer(){

		}
		
		virtual void setupScene(string layerName, float width, float height){
			sceneName = layerName;
			setupScreenDimensions(width, height);
		}
		
		virtual void update(float speedPct){

			for(int k = 0; k < elements.size(); k++){
				elements[k]->update(speedPct);
			}
			
			for(int k = 0; k < elementsNoShader.size(); k++){
				elementsNoShader[k]->update(speedPct);
			}	
			
		}	
		
		virtual void checkInteraction( trackerManager * tracker ){
			for(int k = 0; k < elements.size(); k++){
				elements[k]->checkInteraction(tracker);
			}
			for(int k = 0; k < elementsNoShader.size(); k++){
				elementsNoShader[k]->checkInteraction(tracker);
			}			
		}				

		virtual void drawIntoShader(){

			ofPushStyle();
			
				for(int k = 0; k < elements.size(); k++){
					ofSetColor(255);		
					ofEnableAlphaBlending();	
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);	
					elements[k]->drawIntoShader();
				}

			ofPopStyle();
		}
				
		virtual void draw(){

			ofPushStyle();
			
				for(int k = 0; k < elements.size(); k++){
					ofSetColor(255);		
					ofEnableAlphaBlending();	
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);	
					elements[k]->draw();
				}

			ofPopStyle();
		}

		virtual void drawNoShader(){

			ofPushStyle();
			ofSetColor(255);		
			ofEnableAlphaBlending();

				for(int k = 0; k < elementsNoShader.size(); k++){
					elementsNoShader[k]->draw();
				}
				
			ofPopStyle();
		}

		virtual void addElementNoShader( vector <baseLayer *> & _elements ){
			for(int k = 0; k < _elements.size(); k++){
				elementsNoShader.push_back( _elements[k] );
			}
		}	
		
		virtual void addElementNoShader( baseLayer * _element ){
			elementsNoShader.push_back( _element );
		}		
		
		virtual void eventsIn(layerMessage & data){
//			cout << " eventsIn not implemented " << endl;  
		}
		
		vector <baseLayer *> elementsNoShader;		
		string sceneName;
		
};