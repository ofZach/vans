#include "advImageSequence.h"

//------------------------------------------------
advImageSequence::advImageSequence(){
    num = 0;
    seq.clear();
	whichSection = 0;
}

//------------------------------------------------
advImageSequence::~advImageSequence(){
    seq.clear();
    dirList.reset();
}

//------------------------------------------------
bool advImageSequence::loadSeq(string directory, string ext){
    seq.clear();
    dirList.reset();
	sections.clear();
    dirList.allowExt(ext);
    num = dirList.listDir(directory);
    if( num <= 0 )return false;
	
	if( directory[directory.length()-1] != '/'){
		directory += "/";
	}
	
	if( !xml.loadFile(directory + "_info.xml") ){
		sections.push_back( seqSection( "default", 0, num-1, false ) );
	}

	int numTags = xml.getNumTags("section");
	for(int i = 0; i < numTags; i++){
		string secName = xml.getValue("section:name", "none", i);
		int startFrame = xml.getValue("section:start", 0, i);
		int endFrame   = xml.getValue("section:end", 0, i);
		bool bLoop	   = xml.getValue("section:loop", false, i);
		
		//printf("name: %s = %i %i \n", secName.c_str(), startFrame, endFrame);
		
		sections.push_back( seqSection( secName, startFrame, endFrame, bLoop ) );
	}
	
	if( numTags > 0)currentSection = sections[0];

    seq.assign(num, ofImage());
	paths.assign(num,"");
    for(int i = 0; i < num; i++){
		//cout << "loading " << dirList.getPath(i) << endl;
		paths[i] = dirList.getPath(i);
		if( i <= 1 ){
			seq[i].loadImage(paths[i]);
		}
	}

	whichSection = 0;

    return true;
}

bool advImageSequence::setSection(string sectionName){
	for(int i = 0; i < sections.size(); i++){
		if( sections[i].name == sectionName){
			currentSection = sections[i];
			whichSection = i;
			return true;
		}
	}
	return false;
}

bool advImageSequence::setSection(int _whichSection){
	if( _whichSection >= 0 && _whichSection < sections.size() ){
		currentSection = sections[_whichSection];
		whichSection = _whichSection;
		return true;
	}
	return false;
}

//---------------------------------------------
int advImageSequence::getNumImages(){
   return seq.size();
}

//----------------------------------------------
ofImage * advImageSequence::getPct(float pct){
    pct = ofClamp(pct, 0, 1);

    if( seq.size() > 0){
        float numImages = seq.size();
		
		int which = ofMap(pct, 0, 1, currentSection.startFrame, currentSection.endFrame);
		ofClamp(which, 0, MAX(0,numImages-1) );
		
		if( seq[which].getWidth() == 0.0 ){
			seq[which].loadImage(paths[which]);
		}
		//seq[which].checkNeedsLoad();
		
        //printf("returning %i \n", which);
        return &seq[which];
    }else{
       // printf("EMPTY MOFO! getPct seq.size is %i, pct is %f ", seq.size(), pct);	
	}
    return &empty;
}


//----------------------------------------------
ofImage * advImageSequence::getFrame(int which){
    if( which >= 0 && which < seq.size()){
        //printf("returning %i \n", which);
		//seq[which].checkNeedsLoad();
		
		if( seq[which].getWidth() == 0.0 ){
			seq[which].loadImage(paths[which]);
		}		
		
        return &seq[which];
    }else{
       // printf("EMPTY MOFO! getFrame seq.size is %i, which is %i ", seq.size(), which);	
	}
    return &empty;
}

