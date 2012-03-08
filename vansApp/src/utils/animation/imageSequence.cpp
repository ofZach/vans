/*
 *
 * Copyright Theodore Watson 2010
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "imageSequence.h"

//------------------------------------------------
imageSequence::imageSequence(){
    num = 0;
    seq.clear();
}

//------------------------------------------------
imageSequence::~imageSequence(){
    seq.clear();
    dirList.reset();
}

//------------------------------------------------
bool imageSequence::loadSeq(string directory, string ext){
    seq.clear();
    dirList.reset();
    dirList.allowExt(ext);
    num = dirList.listDir(directory);
    if( num <= 0 )return false;

    seq.assign(num, ofImage());

    for(int i = 0; i < num; i++){
        seq[i].loadImage(dirList.getPath(i));
    }

    return true;
}

//---------------------------------------------
int imageSequence::getNumImages(){
   return seq.size();
}

//----------------------------------------------
ofImage * imageSequence::getPct(float pct){
    pct = MAX(0, pct);
    pct = MIN(1, pct);

    if( seq.size() > 0){
        float numImages = seq.size();
        int which = (float)numImages * pct * 0.999;
        //printf("returning %i \n", which);
        return &seq[which];
    }
    return &empty;
}


//----------------------------------------------
ofImage * imageSequence::getFrame(int which){
    if( which >= 0 && which < seq.size()){
        //printf("returning %i \n", which);
        return &seq[which];
    }
    return &empty;
}


