

#include "ofMain.h"
#include "ofxOpenCv.h"

//  I track colors using percetual RGB distance, as described here: 
//  http://www.compuphase.com/cmetric.htm




typedef struct {
    int val;
    int spread;
} colorTrackingRange;

class colorTracker{
    
    
public: 
    
    
    colorTracker(){
        allocate(640,480);
        bUseHSV = true;
        
        bUseHueRange = true;
        bUseSatRange = true;
        bUseValRange = true;
        
        hueRange.spread = 100;
        satRange.spread = 100;
        valRange.spread = 100;
        
        bJustBottomHalf = true;
        bUseAlphaMask = true;
    }
    
    void allocate(int w, int h){
        trackingResults.allocate(w,h);
        hsv.setUseTexture(false);
        hsv.allocate(w,h);
    }
    
    
    void setTrackedColor(ofColor _toTrack){
        trackedColor = _toTrack;
    }
    
    void track(ofxCvColorImage & colorPixels, ofxCvGrayscaleImage & alphaMask); 
    
    
    //--------------------------------------------
    // these are variable for the tracking: 
    int distance;
    ofColor trackedColor;

    //--------------------------------------------
    unsigned char * grayPixels;
    ofxCvGrayscaleImage trackingResults;
    ofxCvColorImage color;
    
    
    bool bUseHSV;
    
    ofxCvColorImage hsv;
    
    // this is min / max, but designed to be cyclical for HUE, etc. 
    
    colorTrackingRange   hueRange;
    colorTrackingRange   satRange;
    colorTrackingRange   valRange;
    
    bool bUseHueRange;
    bool bUseSatRange;
    bool bUseValRange;
    
    
    bool bJustBottomHalf;
    bool bUseAlphaMask;
    
    
};


