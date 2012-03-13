#include "trackerManager.h"
	
void trackerManager::setup(float w, float h){
	
	screenW = w;
	screenH = h;
	
	smoothedDiff = 0.0;
	preArea = 0.0;
	normalizedSmoothedSpeed = 0.0;
	smoothedDepth = 0.0;
	normalizedSmoothDepth = 0.0;
					
	timeSincePts = 0.0;
	t1 = -1.0;
	curId = 0;
	statePct = 0.0;
		
	state = TRACKER_NONE_LONG;
}	

void trackerManager::update( ofxCvColorImage & _rgb, ofxCvGrayscaleImage & _alpha, ofxCvGrayscaleImage & depth){
	
	if( _rgb.getWidth() == 0 ) return;
	
	if( color.getWidth() != _rgb.getWidth() ){
		color.clear(); 
		alpha.clear(); 
		color.allocate(_rgb.getWidth(), _rgb.getHeight());
		alpha.allocate(_alpha.getWidth(), _alpha.getHeight());
		rgbaPix.allocate(_rgb.getWidth(), _rgb.getHeight(), OF_IMAGE_COLOR_ALPHA);
		depthImage.allocate(depth.getWidth(), depth.getHeight());
	}
	
	depthImage = depth;
	
	// APPLIES A REVERSE GRADIENT TO THE IMAGE TO REMOVE FLOOR
	
	if( guiPtr->getValueB("floorFilter") ){
	
		unsigned char * pix = _alpha.getPixels();
		int numW = _alpha.getWidth();
		int numH = _alpha.getHeight();
		
		int startY = ofClamp(numH - (guiPtr->getValueF("floorStartY")-1), 0, numH-1);
		
		float valPerLine = guiPtr->getValueI("floorFilterVal");
		float valReduce	 = valPerLine;
		for(int y = startY; y < numH; y++){
			for(int x = 0; x < numW; x++){
				int index = x + y * numW; 
				
				int p = pix[index];
				p -= valReduce;
				if( p < 0 ){ 
					p = 0;
				}
				
				pix[index] = p;
			}
			valReduce += valPerLine;
		}
		
		_alpha.flagImageChanged();
	}
		
	
	alpha = _alpha;
	
	alpha.blur(3); 
	alpha.threshold(guiPtr->getValueF("alphaThresh1"));
	alpha.blur(guiPtr->getValueI("alphaBlur")); 
	alpha.threshold(guiPtr->getValueF("alphaThresh2"), false);
	alpha.blur(3); 		

	color = _rgb; 
	
	//BLIT THAT SHIT
	
	unsigned char * rgb = color.getPixels();
	unsigned char * rgba = rgbaPix.getPixels();
	unsigned char * a = alpha.getPixels();
	int numPix = rgbaPix.getWidth() * rgbaPix.getHeight();
	
	int k = 0;
	int j = 0; 
	for(int i = 0; i < numPix * 4; i+= 4){
		rgba[i]   =  rgb[k];
		rgba[i+1] =  rgb[k+1];
		rgba[i+2] =  rgb[k+2];
		rgba[i+3] =  a[j];
		k+=3;
		j++;
	}
	
	rgbaPix.update();
	
	finder.findContours(alpha, 30*30, 640*480, 10, false, true);
	
	vector <ofPoint> fivePts;
	fivePts.assign(5, ofPoint());
	
	float depthVal = 0.0;
		
	//AVG DEPTH OF PERSON
	
	if( finder.blobs.size() ){
		ofRectangle bounds = finder.blobs[0].boundingRect;
		
		fivePts[0] = finder.blobs[0].centroid;
		fivePts[1] = ofPoint(bounds.x, bounds.y);
		fivePts[2] = ofPoint(bounds.x + bounds.width , bounds.y);
		fivePts[3] = ofPoint(bounds.x + bounds.width , bounds.y + bounds.height);
		fivePts[4] = ofPoint(bounds.x , bounds.y + bounds.height);
		
		ofPixelsRef pixRef = _alpha.getPixelsRef();
		
		float pix1 = pixRef.getColor(finder.blobs[0].centroid.x, finder.blobs[0].centroid.y).r;
		float pix2 = pixRef.getColor(finder.blobs[0].centroid.x - (bounds.width*0.15) , finder.blobs[0].centroid.y - (bounds.height*0.2) ).r;
		float pix3 = pixRef.getColor(finder.blobs[0].centroid.x + (bounds.width*0.15), finder.blobs[0].centroid.y + (bounds.height*0.2) ).r;
		
		depthVal = MAX(pix1, pix2);
		depthVal = MAX(depthVal, pix3);
	}
	
	float depthSmooth = guiPtr->getValueF("depthSmooth");
	
	smoothedDepth *= depthSmooth;
	smoothedDepth += depthVal * (1.0-depthSmooth);
	normalizedSmoothDepth = ofMap( smoothedDepth, 0, 255, 0.0, 1.0, true );
	
	//cout << " normalizedSmoothDepth " << normalizedSmoothDepth << endl; 
	
	
	//AVG MOVEMENT IN THE FRAME
	
	if( prePts.size() != 5 ){
		prePts.assign(5, ofPoint());
		prePts = fivePts;
	}	
	
	float diff = 0.0;
	for(int i = 0; i < fivePts.size(); i++){
		diff += ( fivePts[i] - prePts[i] ).length();
	}
	diff /= 5.0; 
	
	float smoothPct = guiPtr->getValueF("speedSmoothPct");
	
	smoothedDiff *= smoothPct;
	smoothedDiff += diff * (1.0-smoothPct);
	
	normalizedSmoothedSpeed = ofMap(smoothedDiff, guiPtr->getValueF("minSmoothedSpeed"), guiPtr->getValueF("maxSmoothedSpeed"), 0, 1, true);

	prePts = fivePts;

}

void trackerManager::recordFrames(bool bTrue){
	
	if( bTrue ){
		if( sessionStr == "" ){
			sessionStr = "recordings/rec-"+ofGetTimestampString() + "/";
			
			if( !ofDirectory::doesDirectoryExist(sessionStr) ){
				ofDirectory::createDirectory( sessionStr, true, true);
			}
		}
		rgbaPix.saveImage(sessionStr + "f-"+ofToString(1000000+ofGetFrameNum()) + ".png", OF_IMAGE_QUALITY_BEST);
	}else{
		sessionStr = "";
	}
}

void trackerManager::draw(float x, float y){

}

void trackerManager::draw(float x, float y, float w, float h){

}
	
void trackerManager::debugDraw(){
	ofPushStyle();
	ofSetColor(255);
	ofNoFill();
		
	ofPopStyle();
}

		