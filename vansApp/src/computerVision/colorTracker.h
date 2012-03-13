

#include "ofMain.h"
#include "ofxOpenCv.h"

//  I track colors using percetual RGB distance, as described here: 
//  http://www.compuphase.com/cmetric.htm



class colorTracker{
    
    
public: 
    
    
    colorTracker(){
        allocate(640,480);
    }
    
    void allocate(int w, int h){
        trackingResults.allocate(w,h);
    }
    
    
    void setTrackedColor(ofColor _toTrack){
        trackedColor = _toTrack;
    }
    
    void track(ofxCvColorImage & colorPixels); 
    
    
    //--------------------------------------------
    // these are variable for the tracking: 
    int distance;
    ofColor trackedColor;

    //--------------------------------------------
    unsigned char * grayPixels;
    ofxCvGrayscaleImage trackingResults;
    ofxCvColorImage color;
    
    
};


