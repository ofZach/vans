
#pragma once
#include "ofMain.h"

//TODO: should this be a pointer?
typedef std::pair<string, ofSoundPlayer> sndPair;

class easySoundPlayer{
	public:
	
	//-----------------
	void addSound(string filename, string uniqueKey, bool multiplay = false, bool looping = false, bool streaming = false);
	void play(string uniqueKey, float vol = 1.0, float pitch = 1.0, float pan = 0.0);
	void stop(string uniqueKey);
	void update();
	bool playing(string uniqueKey);

	protected:
		
		map <string, ofSoundPlayer> snds;

};