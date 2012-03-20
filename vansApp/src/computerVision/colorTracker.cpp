
#include "colorTracker.h"


#include <stdlib.h>
#include <stdio.h>

#define MIN3(x,y,z)  ((y) <= (z) ? \
((x) <= (y) ? (x) : (y)) \
: \
((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
((x) >= (y) ? (x) : (y)) \
: \
((x) >= (z) ? (x) : (z)))


//struct rgb_color {
//    unsigned char r, g, b;    /* Channel intensities between 0 and 255 */
//};
//
//struct hsv_color {
//    unsigned char hue;        /* Hue degree between 0 and 255 */
//    unsigned char sat;        /* Saturation between 0 (gray) and 255 */
//    unsigned char val;        /* Value between 0 (black) and 255 */
//};


unsigned char * rgb_to_hsv(unsigned char *  rgb) {
    //struct hsv_color hsv;
    unsigned char hsv[3];
    
    unsigned char rgb_min, rgb_max;
    rgb_min = MIN3(rgb[0], rgb[1], rgb[2]);
    rgb_max = MAX3(rgb[0], rgb[1], rgb[2]);
    
    hsv[2] = rgb_max;
    if (hsv[2] == 0) {
        hsv[0] = hsv[1] = 0;
        return hsv;
    }
    
    hsv[1] = 255*long(rgb_max - rgb_min)/hsv[2];
    if (hsv[1] == 0) {
        hsv[0] = 0;
        return hsv;
    }
    
    /* Compute hue */
    if (rgb_max == rgb[0]) {
        hsv[0] = 0 + 43*(rgb[1] - rgb[2])/(rgb_max - rgb_min);
    } else if (rgb_max == rgb[1]) {
        hsv[0] = 85 + 43*(rgb[2] - rgb[0])/(rgb_max - rgb_min);
    } else /* rgb_max == rgb[2] */ {
        hsv[0] = 171 + 43*(rgb[0] - rgb[1])/(rgb_max - rgb_min);
    }
    
    return hsv;
}





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



void colorTracker::track(ofxCvColorImage & colorPixels, ofxCvGrayscaleImage & alphaMask){
    if( color.getWidth() == 0 ){
		color.allocate( colorPixels.getWidth(), colorPixels.getHeight() );
	}
	
	color = colorPixels;
	//color.blur(11);
    
    if (bUseHSV == false){
        unsigned char colorTrack[3];
        colorTrack[0] = trackedColor.r;
        colorTrack[1] = trackedColor.g;
        colorTrack[2] = trackedColor.b;
     
        unsigned char * testPixels = color.getPixels();
        unsigned char * resultPixels = trackingResults.getPixels();
        unsigned char * alphaPixels = alphaMask.getPixels();

        int w = color.getWidth();
        int h = color.getHeight();
        
        int wh_div_2 = w*h / 2;
        
        memset(resultPixels, 0, w*h);
        
        int start = 0;
        if (bJustBottomHalf == true) start = wh_div_2;
        
        for (int i = start; i < w*h; i++){
            
            if (bUseAlphaMask && alphaPixels[i] == 0){
                continue;
            }
            
            
            resultPixels[i] = colorDistance(colorTrack, (testPixels + i*3), distance);
        }
        
        trackingResults.flagImageChanged();
        
    } else {
        
        //hsv = color;
        //hsv.convertRgbToHsv();
        
        
        hueRange.val = trackedColor.getHue();
        satRange.val = trackedColor.getSaturation();
        valRange.val = trackedColor.getBrightness();
        
        unsigned char * testPixels = color.getPixels();
        unsigned char * resultPixels = trackingResults.getPixels();
        unsigned char * alphaPixels = alphaMask.getPixels();

        int w = color.getWidth();
        int h = color.getHeight();
        
        memset(resultPixels, 0, w*h);
        
        int hue, sat, val;
        float hueDist, satDist, valDist;
        bool bOk;
        int wh_div_2 = w*h / 2;
        int start = 0;
        if (bJustBottomHalf == true) start = wh_div_2;
        
        ofColor temp;
        for (int i = start; i < w*h; i++){
            
            if (bUseAlphaMask && alphaPixels[i] == 0){
                continue;
            }
            
            
            bOk = true;
            
            temp.set(testPixels[i*3], testPixels[i*3+1], testPixels[i*3+2]);
            //unsigned char * hsv = rgb_to_hsv(testPixels + i*3);
            
            hue = temp.getHue(); // testPixels[i*3];
            sat = temp.getSaturation(); // testPixels[i*3+1];
            val = temp.getBrightness(); // testPixels[i*3+2];
            
            printf("--- %i %i %i \n", hue, sat, val);
            // handle hue distance!  make it small, like an angle change. 
            
            if (bUseHueRange){
                hueDist= fabs(hue - hueRange.val);
                if (hueDist > 128){
                    hueDist -= 255;  // the shorter way around 255. 
                }
                hueDist = fabs(hueDist);    // hue dist could be negative, fix this. 
                if (hueDist > hueRange.spread){
                    bOk = false;
                }
            }
            
            if (bUseSatRange){
                satDist= fabs(sat - hueRange.val);
                if (satDist > satRange.spread){
                    bOk = false;
                }
            }
            
            if (bUseValRange){
                valDist= fabs(val - valRange.val);
                if (valDist > valRange.spread){
                    bOk = false;
                }
            }
            
            if (bOk){
                resultPixels[i] = 255; 
            } else {
                resultPixels[i] = 0;
            }
            
            
            //bool bOk = false;
            //resultPixels[i] = colorDistance(colorTrack, (testPixels + i*3), distance);
        }
        
        trackingResults.flagImageChanged();
        
    }

}
    