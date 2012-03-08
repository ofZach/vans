
#pragma once
#include "ofMain.h"


class customSoundPlayer : public ofSoundPlayer{
	public:
		float internalVol;
		float targetVol;
		bool bShort;
		
		customSoundPlayer(){
			internalVol = 0.0;
			targetVol   = 0.0;
			bShort      = false;
		}
		
		void setTargetVol(float target){
			targetVol = target;
		}
		
		void forceReTrigger(){
			setPosition(0.0);
			play();
			printf("--- playing forced!\n");
		}
		
		bool update(float globalAmnt, float rate){
			internalVol = ofLerp(internalVol, targetVol, rate);
			setVolume(internalVol * globalAmnt);
			return false;
		}
};

class backgroundSoundManager{


	public:
	
	void setup(string directory){

		loadFromDirectory(directory+"rise/", rise);
		loadFromDirectory(directory+"fall/", fall);
		loadFromDirectory(directory+"both/", both);
		
	}
	
	void play(){
		for(int i = 0; i < rise.size(); i++){
			rise[i].play();
			rise[i].setVolume(0.0);
			rise[i].setLoop(true);
			rise[i].update(0.0, 1.0);
		}
		for(int i = 0; i < fall.size(); i++){
			fall[i].play();
			fall[i].setVolume(0.0);
			fall[i].setLoop(true);	
			fall[i].update(0.0, 1.0);					
		}
		for(int i = 0; i < both.size(); i++){
			both[i].play();
			both[i].setVolume(0.0);
			both[i].setLoop(true);	
			both[i].update(0.0, 1.0);													
		}
					
		ofSoundUpdate();
	}
	
	void update(float riseFallPct){
		
		bool bForceTrigger = false;
		
		for(int i = 0; i < both.size(); i++){
			both[i].setTargetVol(0.2 + ofNoise(i*10, ofGetElapsedTimef() * 0.01) * 0.6 );
			both[i].update(1.0, 0.1);
			
//			if( both[i].update(1.0, 0.1) && !both[i].bShort){
//				bForceTrigger = true;
//			}
//			
//			if( bForceTrigger ){
//				both[i].forceReTrigger();
//			}
		}
		
		for(int i = 0; i < rise.size(); i++){
			if( bForceTrigger ){
				rise[i].forceReTrigger();
			}
			
			if( riseFallPct > 0.6 ){
				rise[i].setTargetVol(riseFallPct);
			}
			if( riseFallPct < 0.4 ){
				rise[i].setTargetVol(0.0);
			}
			
			float gVol = 1.0;
			float uVal = ofSignedNoise(i*10, ofGetElapsedTimef()*0.03) ;
			gVol = ofMap(uVal, -0.45, -0.25, 0.0, 1.0, true);
						
			rise[i].update(gVol, 0.07);
		}
		
		for(int i = 0; i < fall.size(); i++){
			if( bForceTrigger ){
				fall[i].forceReTrigger();
			}
						
			if( riseFallPct > 0.6 ){
				fall[i].setTargetVol(0.0);
			}
			if( riseFallPct < 0.4 ){		
				fall[i].setTargetVol((1.0-riseFallPct));
			}
			
			
			float gVol = 1.0;
			float uVal = ofSignedNoise(i*10, ofGetElapsedTimef()*0.03) ;
			gVol = ofMap(uVal, -0.45, -0.25, 0.0, 1.0, true);
			
			fall[i].update(gVol, 0.07);
		}		
	}
	

	protected:
		void loadFromDirectory(string path, vector <customSoundPlayer> & vec){
			ofDirectory dirList;
			int num = dirList.listDir(path);
			
			float startTime = ofGetElapsedTimef();
			
			vec.assign(num, customSoundPlayer());
			for(int i = 0; i < vec.size(); i++){
			
				//TODO: check if streaming eats cpu for lots of mp3s 
				vec[i].loadSound(dirList.getPath(i), true);
				vec[i].setLoop(false);
				
				if( strstr(dirList.getName(i).c_str(), "2 cycles") != NULL ){
					vec[i].bShort = false;
				}else{
					vec[i].bShort = true;
				}
				
			}	
			printf("loadFromDirectory time %f\n",ofGetElapsedTimef()-startTime);
		}	

	vector <customSoundPlayer> rise;
	vector <customSoundPlayer> fall;
	vector <customSoundPlayer> both;
	
};