
#include "easySoundPlayer.h"

//-----------------
void easySoundPlayer::addSound(string filename, string uniqueKey, bool multiplay, bool looping, bool streaming){			
	snds.insert(sndPair(uniqueKey, ofSoundPlayer()));
	snds[uniqueKey].loadSound(filename, streaming);
	snds[uniqueKey].setMultiPlay(multiplay);
	snds[uniqueKey].setLoop(looping);
}

void easySoundPlayer::play(string uniqueKey, float vol, float pitch, float pan){
	if( snds[uniqueKey].isLoaded() == false )return;

	snds[uniqueKey].setPan(pan);	 	
	snds[uniqueKey].play();
	snds[uniqueKey].setVolume(vol);
	snds[uniqueKey].setSpeed(pitch);
	snds[uniqueKey].setPan(pan);	 
}

void easySoundPlayer::stop(string uniqueKey){
	if( snds[uniqueKey].isLoaded() == false )return;	
	snds[uniqueKey].stop();
}	

bool easySoundPlayer::playing(string uniqueKey){
	if( snds[uniqueKey].isLoaded() == false )return false;	
	return snds[uniqueKey].getIsPlaying();
}	


//-----------------	
void easySoundPlayer::update(){
	ofSoundUpdate();
}
