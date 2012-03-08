
#pragma once


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "imageResource.h"

class seqSection{
	public:
		seqSection(){
			name = "";
			startFrame = 0;
			endFrame = 0;
			bLoop = false;
		}
		seqSection(string _name, int _start, int _end, bool _bLoop){
			name		= _name;
			startFrame	= _start;
			endFrame	= _end;
			bLoop		= _bLoop;
		}
		seqSection( const seqSection &in ){
			name		= in.name;
			startFrame	= in.startFrame;
			endFrame	= in.endFrame;
			bLoop		= in.bLoop;
		}
		
		bool bLoop;
		string name;
		int startFrame;
		int endFrame;
};


class advImageSequence{
    public:

		//------------------------------------------------
		advImageSequence();
		virtual ~advImageSequence();

		bool loadSeq(string directory, string ext);
		int getNumImages();

		ofImage * getPct(float pct);
		ofImage * getFrame(int which);
		
		bool setSection(string sectionName);
		bool setSection(int whichSection);

		ofDirectory dirList;
		vector <ofImage> seq;
		ofxXmlSettings xml;
		
		vector <seqSection> sections;
		vector <string> paths;
		seqSection currentSection;

		int whichSection;

		ofImage empty;
		int num;
	
};
