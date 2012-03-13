
#include "colorTracker.h"




inline unsigned char colorDistance( unsigned char * colorPixA, unsigned char * colorPixB, int distance){
    
    
    /*
    long rmean = ( (long)e1.r + (long)e2.r ) / 2;
    long r = (long)e1.r - (long)e2.r;
    long g = (long)e1.g - (long)e2.g;
    long b = (long)e1.b - (long)e2.b;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
    */
    
    long rmean = ( (long)colorPixA[0] + (long)colorPixB[0] ) / 2;
    long r = (long)colorPixA[0] - (long)colorPixB[0];
    long g = (long)colorPixA[1] - (long)colorPixB[1];
    long b = (long)colorPixA[2] - (long)colorPixB[2];
    
    return (sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8))) > distance ? 0 : 255;

}



void colorTracker::track(ofxCvColorImage & colorPixels){
    if( color.getWidth() == 0 ){
		color.allocate( colorPixels.getWidth(), colorPixels.getHeight() );
	}
	
	color = colorPixels;
	color.blur(11);
	
    unsigned char colorTrack[3];
    colorTrack[0] = trackedColor.r;
    colorTrack[1] = trackedColor.g;
    colorTrack[2] = trackedColor.b;
    
    unsigned char * testPixels = color.getPixels();
    unsigned char * resultPixels = trackingResults.getPixels();

    int w = colorPixels.getWidth();
    int h = colorPixels.getHeight();
    
    for (int i = 0; i < w*h; i++){
        resultPixels[i] = colorDistance(colorTrack, (testPixels + i*3), distance);
    }
    
    trackingResults.flagImageChanged();
    
    //trackingResults.erode_3x3();
    //trackingResults.dilate_3x3();
}
    