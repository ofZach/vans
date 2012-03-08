#include "imageResource.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

map <string, ofImage> imgR;

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"

using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;

//--------------------------------------------------------------
string getHash(string input){
	std::string passphrase("paintedMirror"); // HMAC needs a passphrase
	
	HMACEngine<MD5Engine> hmac(passphrase); // we'll compute a MD5 Hash
	hmac.update(input);
	
	const DigestEngine::Digest& digest = hmac.digest(); // finish HMAC computation and obtain digest
	std::string digestString(DigestEngine::digestToHex(digest)); // convert to a string of hexadecimal numbers

	return digestString;	
}

imageResource::imageResource(){
	bNeedsLoad = false;
	debugColor = 0;
	scale = 1.0;
	key = "NULL";
	width = 0.0;
	height = 0.0;
}

bool imageResource::loadImage( string filename, bool bDeferredLoad, bool bMakeContours ){
	originalPath = filename;
	key = getHash(originalPath); 
	
	if( !bDeferredLoad || bMakeContours ){
				
		if( imgR.count(key) == 0 ){
			//cout << "++ creating key  " << key << " for " << originalPath << endl;
			imgR[key].loadImage(filename);
		}else{
			//cout << "-- key  " << key << " already exists for " << originalPath << endl;
		}
		
		width = imgR[key].getWidth();
		height = imgR[key].getHeight();
		
		lastDraw.set(0, 0, width, height);
		
		if( bMakeContours ){

			checkLoadXml( filename + ".xml" );
			
			if( contours.size() == 0 ){
				makeContoursFromAlpha();
			}else{
				//cout << " we loaded a cached copy " << endl; 
			}
		
		}
	
		bNeedsLoad = false;
	}else{
		bNeedsLoad = true;	
	}

	debug = false;
}

void imageResource::checkNeedsLoad(){
	if( bNeedsLoad ){
		if( imgR.count(key) == 0 ){
			cout << "++ defered load - creating key " << key << " for " << originalPath << endl;			
			imgR[key].loadImage(originalPath);
		}else{
			//cout << "-- defered load - key " << key << " already exists for " << originalPath << endl;
		}

		width = imgR[key].getWidth();
		height = imgR[key].getHeight();		
		lastDraw.set(0, 0, width, height);
		bNeedsLoad = false;
	}
}

unsigned char * imageResource::getPixels(){		
	checkNeedsLoad();
	return imgR[key].getPixels();
}

ofPixelsRef imageResource::getPixelsRef(){		
	checkNeedsLoad();
	return imgR[key].getPixelsRef();
}

void imageResource::setAnchorPercent(float xPct, float yPct){
	checkNeedsLoad();
	imgR[key].setAnchorPercent(xPct, yPct);
	anchor.set(xPct, yPct);
}

void imageResource::draw(float x, float y){
	checkNeedsLoad();
	imageResource::draw(x, y, width, height);
}

void imageResource::draw(float x, float y, float z){
	checkNeedsLoad();
	imageResource::draw(x, y, z, width, height);
}

void imageResource::draw(float x, float y, float w, float h){
	checkNeedsLoad();
	imageResource::draw(x, y, 0.0, w, h);
}

void imageResource::draw(float x, float y, float z, float w, float h){
	w *= scale;
	h *= scale;

	checkNeedsLoad();
	imgR[key].draw(x,y,z,w,h);
	lastDraw.set(x,y,w,h);
	
	if( ofGetKeyPressed('d') )debug = true;
	else debug = false;
	
//	if( debug && debugImg.getWidth() > 0 ){
//		ofSetColor(255, 255, 255, 255);
//		debugImg.setAnchorPercent(anchor.x, anchor.y);
//		debugImg.draw(x, y, w, h);
//	}
	
	if( debug && contours.size() && w > 0.0 && h > 0.0 ){
	
		vector <ofxCvBlob> tmpContour = getContours();
	
		ofPushStyle();
		
		if( debugColor == 0 ){
			ofSetColor(255, 0, 255);
		}else if( debugColor == 1 ){
			ofSetColor(255, 255, 0);		
		}else if( debugColor == 2 ){
			ofSetColor(0, 255, 255);		
		}else if( debugColor == 3 ){
			ofSetColor(255, 0, 0);		
		}else if( debugColor == 4 ){
			ofSetColor(255, 190, 60);		
		}
		
//		ofPushMatrix();
//		ofTranslate(x - anchor.x * (float)width*scale, y - anchor.y * (float)height*scale, 0);
//		ofScale( ( w / (float)width ), (  h / (float)height ), 1.0 );
			
			glPointSize(2.0);
			for(int k = 0; k < tmpContour.size(); k++){
				glBegin(GL_POINTS);
				for(int i = 0; i < tmpContour[k].pts.size(); i++){
					glVertex2f(tmpContour[k].pts[i].x, tmpContour[k].pts[i].y);
				}
				glEnd();
				
				ofCircle( tmpContour[k].centroid , 10 );
			}
			glPointSize(1.0);
			
//		ofPopMatrix();
		ofPopStyle();
	}
	
}


void transformPt(ofPoint & p, ofRectangle lastDraw, float width, float height, ofPoint anchor){
	p.x -= anchor.x * (float)width;
	p.y -= anchor.y * (float)height;

	p.x *= (lastDraw.width / (float)width);
	p.y *= (lastDraw.height / (float)height);
	
	p.x += lastDraw.x;
	p.y += lastDraw.y;
}

//TODO: - implement caching if drawing hasn't changed.
vector <ofxCvBlob> imageResource::getContours(){
	vector <ofxCvBlob> contourTmp;
	
	if( contours.size() && lastDraw.width > 0.0 && lastDraw.height > 0.0 ){
		contourTmp.assign(contours.size(), ofxCvBlob() );	
				
		for(int k = 0; k < contourTmp.size(); k++){
		
			ofPoint avg;
			for(int i = 0; i < contours[k].pts.size(); i++){
				ofPoint pt = contours[k].pts[i];				
				transformPt( pt, lastDraw, width, height, anchor );			
				contourTmp[k].pts.push_back(pt);
				avg += pt;
			}
			
			avg/=contours[k].pts.size();
			
			contourTmp[k].centroid = avg;
			//transformPt( contourTmp[k].centroid, lastDraw, width, height, anchor );			
		}
	}
	
	return contourTmp;
}
	
void imageResource::checkLoadXml( string xmlPath ){
	ofxXmlSettings xml;
	if( xml.loadFile( xmlPath ) ){
	
		int numTags = xml.getNumTags("contour");
		
		if( numTags > 0 ){
			
			contours.clear();
			contours.assign( numTags, ofxCvBlob() );
			
			for(int i = 0; i < numTags; i++){
				xml.pushTag("contour", i );
				
				int numPts = xml.getNumTags("pt");
				
				for(int j = 0; j < numPts; j++){
					contours[i].pts.push_back( ofPoint( xml.getValue("pt:x", 0.0, j), xml.getValue("pt:y", 0.0, j) ) );
				}
				
				xml.popTag();
			}
			
			cout << " loaded " << numTags << " contours " << endl; 
			
		}
	
	}

}

void imageResource::saveXml( string xmlPath ){
	
	
	if( contours.size() > 0 ){
		
		if( ofFile::doesFileExist( xmlPath ) ){
			ofFile::removeFile( xmlPath );
		}
		
		ofxXmlSettings xml;
				
		for(int i = 0; i < contours.size(); i++){
			xml.addTag("contour");
			
			xml.pushTag("contour", i );
			
			int numPts = contours[i].pts.size();
			
			for(int j = 0; j < numPts; j++){
				
				xml.addTag("pt");
				xml.setValue("pt:x", contours[i].pts[j].x, j);
				xml.setValue("pt:y", contours[i].pts[j].y, j);				
			}
			
			xml.popTag();
		}
		
		xml.saveFile(xmlPath);
		//cout << " saved " << contours.size() << " contours " << endl; 
	}
	
}


void imageResource::makeContoursFromAlpha(){

	if( imgR[key].type != OF_IMAGE_COLOR_ALPHA ){
		//cout << " IMAGE " << originalPath << " is not alpha " << endl; 
		return;
	}
	
	ofxCvGrayscaleImage gray; 
	gray.allocate(width, height);
	//debugImg.allocate(width, height);
	
	unsigned char * pixSrc = imgR[key].getPixels();
	ofPixels pixDst;
	pixDst.allocate(width, height, OF_PIXELS_MONO);
	
	int dst = 0;
	int src = 0;
	for(int y = 0; y < height; y++ ){
		for(int x = 0; x < width; x++ ){
			src = ( x + y * width ) * 4;
			
			pixDst[dst] = pixSrc[src+3];
			
			dst++;
		}
	}
	
	gray.setFromPixels(pixDst.getPixels(), width, height);
	gray.flagImageChanged();
	
	//debugImg = gray;
	gray.threshold( 180 );
	
	ofxCvContourFinder finder;
	finder.findContours( gray, 20 * 20, width*height, 20, true, true );
	
	contours = finder.blobs;
	
	saveXml( originalPath + ".xml" );
	
	//cout << "found " << contours.size() << " blobs " << " width is " << width << " height is " << height  << endl; 
	
}
		
		