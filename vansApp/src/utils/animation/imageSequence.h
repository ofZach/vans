
#pragma once


#include "ofMain.h"

class imageSequence{
    public:

    //------------------------------------------------
    imageSequence();
    virtual ~imageSequence();

    bool loadSeq(string directory, string ext);
    int getNumImages();

    ofImage * getPct(float pct);
    ofImage * getFrame(int which);

    ofDirectory dirList;
    vector <ofImage> seq;

    ofImage empty;
    int num;
};
