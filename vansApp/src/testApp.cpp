#include "testApp.h"

ofxControlPanel * guiPtr;
trackerManager * trackManPtr;

bool bDebug = false;

float screenW = 1024;
float screenH = 768;

bool bFrameNew = false;
bool hide = true;

bool bFirstSetup = false;
bool bNeedsSetup = true;
bool bRecordFrames = false;

bool bLaptop = false;
     
//--------------------------------------------------------------
void testApp::setup(){	
	
	ofHideCursor();
	//CGDisplayHideCursor(NULL);

	ofSetFrameRate(30);
	ofSetLogLevel(OF_LOG_ERROR);
	
	if( bFirstSetup ){
	
		vans.setupScreenDimensions(screenW, screenH);
			
		ofBackground(0,0,0);	
		ofSetCircleResolution(180);
		
		ofSeedRandom();
        
		guiPtr = &gui;
		km.setup(gui);

		speedComp = 1.0;
		tracker.setup(screenW, screenH);
			
		ofBackground(127,127,127);		
		ofSetVerticalSync(false);
			
		gui.loadFont("MONACO.TTF", 7);		
		gui.setup("app controls", 0, 0, screenW, screenH, true, false);
		gui.addPanel("kinect", 4, false);
		gui.addPanel("scene params", 4, false);
		//gui.setInvisibleMode(true);

		gui.setWhichPanel(0);
				
		float ratio = ofGetWidth()/ ofGetWindowWidth();
			
		gui.addChartPlotter("speed comp", guiStatVarPointer("speed comp", &speedComp, GUI_VAR_FLOAT, true, 2), 200, 50, 200, 0.1, 8.0);
		
		gui.setWhichColumn(1);
		gui.addChartPlotter("some chart", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 200, 50, 200, 5, 80);
								
		//some dummy vars we will update to show the variable lister object
		elapsedTime		= ofGetElapsedTimef();
		appFrameCount	= ofGetFrameNum();	
		appFrameRate	= ofGetFrameRate();
			
		gui.addSlider("volume", "volume", 0.0, 0.0, 1.0f, false);
		gui.addSlider("maxDistFootMove", "maxDistFootMove", 100, 10, 300, true);
		gui.addSlider("scaleComp", "scaleComp", 1.2, 0.4, 2.4, false);
		gui.addSlider("crazyModeScaleAdd", "crazyModeScaleAdd", 1.2, 1.0, 2.0, false);
		
		//load from xml!
		gui.loadSettings("controlPanelSettings.xml");
		
		//START VISION
		//--------- PANEL 1
		gui.setWhichPanel(1);
			
		gui.setWhichColumn(0);
		gui.addToggle("flip horiz", "flipVideoH", false);
		gui.addToggle("flip vert", "flipVideoV", false);
				
		gui.addSlider("clipMin (cm)", "clipMin", 80, 0, 400, true);
		gui.addSlider("clipMax (cm)", "clipMax", 180, 0, 400, true);
		
		gui.addToggle("floorFilter", false);
		gui.addSlider("floorStartY", 40, 0, 200, true);
		gui.addSlider("floorFilterVal", 1.0, 0.25, 6.0, false );

		gui.addSlider("alphaBlur", 9, 0, 21, true);
		gui.addSlider("alphaThresh1", 40, 0, 255, true);
		gui.addSlider("alphaThresh2", 90, 0, 255, true);
		
		gui.addSlider("speedSmoothPct", 0.95, 0.0, 1.0, false);
		gui.addSlider("minSmoothedSpeed", 2.0, 0.0, 20, false);
		gui.addSlider("maxSmoothedSpeed", 30, 0.0, 150, false);
		gui.addSlider("depthSmooth", 0.93, 0.1, 1.0, false);

		gui.addSlider("videoBrightness", 0, -100, 100, true);
				
		gui.addToggle("filter 255 pixels", "bFilterWhitePixels", false);
		
		gui.addToggle("showTrackerDebug", false);
									
		gui.setWhichColumn(1);
		
		gui.setWhichColumn(2);
		gui.addDrawableRect("kinect live", &km.alphaSmoothed, 320, 240);
		gui.addDrawableRect("kinect live", &km.color, 320, 240);

		gui.setWhichPanel(2);
		gui.setWhichColumn(0);
		gui.addDrawableRect("colorTracker", &tracker, 640, 480*2);

		gui.setWhichColumn(1);
		gui.addToggle("showMasked", true);
		gui.addSlider("minFeetSize", "minFeetSize", 60, 0, 1000, true);
		gui.addSlider("maxFeetSize", "maxFeetSize", 70*70, 100, 150*150, true);
		gui.addSlider("blobTrackerDist", "trackerDist", 30, 4, 400, true);
		
		gui.addSlider("distance", "distance", 50, 0, 255, true);
		gui.addSlider("red", "r", 200, 0, 255, true);
		gui.addSlider("green", "g", 60, 0, 255, true);
		gui.addSlider("blue", "b", 40, 0, 255, true);
		gui.addToggle("useHsv", "bUseHsv", true);
        gui.addToggle("trackHue", "trackHue", true);
        gui.addSlider("hueSpread", "hueSpread", 100, 0, 255, true);
        gui.addToggle("trackSat", "trackSat", true);
        gui.addSlider("satSpread", "satSpread", 100, 0, 255, true);
        gui.addToggle("trackVal", "trackVal", true);
        gui.addSlider("valSpread", "valSpread", 100, 0, 255, true);
		
        gui.addToggle("bottomHalfOnly", "bottomHalfOnly", true);
        gui.addToggle("useAlphaMask", "useAlphaMask", true);
		//--- END VISION				
										
		//SETTINGS AND EVENTS
		//if you want to use events call this after you have added all your gui elements
		gui.setupEvents();
		gui.enableEvents();

		//load from xml!
		gui.loadSettings("controlPanelSettings.xml");
		gui.setSelectedPanel(1);
				
        trackManPtr = &tracker;
		
		//  -- this gives you back an ofEvent for all events in this control panel object
		ofAddListener(gui.guiEvent, this, &testApp::eventsIn);
	
		bNeedsSetup = false;
	}
}

//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){

}

//--------------------------------------------------------------
void testApp::update(){
	if( !bFirstSetup ) return; 
	
	if( ofGetFrameNum() == 40 ){
		if( bLaptop )ofSetFullscreen(false);
		else ofSetFullscreen(true);
	}
			
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();

	gui.update();
	
	if( ofGetFrameNum() < 60 ){
		speedComp = 1.0;
	}else{
		float newVal =  60.0f / ( 1.0 / MAX(0.0001, ofGetLastFrameTime() ) );
		speedComp *= 0.7;
		speedComp += 0.3 * newVal;	
	}
			
	vans.update(speedComp);
	
	km.update();
	
	if( km.bFrameLNew ){
		vans.checkInteraction(&tracker);
		tracker.update(km.getColor(), km.getSmoothedAlpha(), km.getDepth());
		
		tracker.recordFrames(bRecordFrames);
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	
   
    ofBackground(0,0,0);
    
    
    float scale = 1;
    ofPoint offsetPt;
    
    float wScreen = ofGetWidth();
    float hScreen = ofGetHeight();
    
    float aspectUs = 768.0 / 1024.0;
    float aspectThem = hScreen / wScreen;
    
    if (aspectUs < aspectThem){
        scale = wScreen / 1024.0;
        float offset = (hScreen - 768*scale)/2.0;
        offsetPt.set(0, offset,0);
        
        
    } else {
        scale = hScreen / 768.0;
        float offset = (wScreen - 1024*scale)/2.0;
        offsetPt.set(offset, 0,0);
        
    }
    
    
	if( bFirstSetup ){ 
		
        ofPushMatrix();
        ofTranslate(offsetPt);
        ofScale(scale, scale, 1);
        vans.draw();
        ofPopMatrix();

		ofSetColor(255, 255, 255);		
		
        ofPopMatrix();
        if( !hide )gui.draw();			
	} else {
        
    }
		
	bFirstSetup = true;
	if( bNeedsSetup )setup();

    
	if( ofGetKeyPressed('p') ){
		ofSetColor(255,0,255);
		ofDrawBitmapString(ofToString(ofGetFrameRate()), 20,20);
	}
	//tracker.CT.trackingResults.draw(mouseX, mouseY);
    
    
    ofPushMatrix();
    ofTranslate(offsetPt);
    ofScale(scale, scale, 1);
    //vans.debugDraw();

    ofPopMatrix();
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
		
	if( key == 'F' ){
		ofToggleFullscreen();
	}
	
	if( key == 'H' ){
		hide = !hide;
        if( hide ) ofHideCursor();
        else{
			ofShowCursor();
		}
	}
		
	if(key == 'R' ){
		bRecordFrames = !bRecordFrames;
	}
	
	if( key == OF_KEY_LEFT ){
		int newPanel = gui.getSelectedPanel()-1;
		if( newPanel >= 0 ){
			gui.setSelectedPanel(newPanel);
		}
	}	
	if( key == OF_KEY_RIGHT ){
		int newPanel = gui.getSelectedPanel()+1;
		if( newPanel < gui.getNumPanels() ){
			gui.setSelectedPanel(newPanel);
		}
	}	
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//water.repelFrom(x, y, 5.0);
    
    
    
    //tracker.CT.distance = x;
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    
    float scale = 1;
    ofPoint offsetPt;
    
    float wScreen = ofGetWidth();
    float hScreen = ofGetHeight();
    
    float aspectUs = 768.0 / 1024.0;
    float aspectThem = hScreen / wScreen;
    
    if (aspectUs < aspectThem){
        scale = wScreen / 1024.0;
        float offset = (hScreen - 768*scale)/2.0;
        offsetPt.set(0, offset,0);
        
        
    } else {
        scale = hScreen / 768.0;
        float offset = (wScreen - 1024*scale)/2.0;
        offsetPt.set(offset, 0,0);
        
    }

    
/*
 ofTranslate(offsetPt);
 ofScale(scale, scale, 1);
 */
    
    if (button == 2){
        ofPoint xp;
        xp.set(x,y);
        xp -= offsetPt;
        //xp *= scale;
        int xpix = ofMap(xp.x, 0, 1024*scale, 0, tracker.color.getWidth());
        int ypix = ofMap(xp.y, 0, 768*scale, 0, tracker.color.getHeight());
        ofColor temp = tracker.color.getPixelsRef().getColor(xpix, ypix);
        gui.setValueI("r", temp.r);
        gui.setValueI("g", temp.g);
        gui.setValueI("b", temp.b);
        
    }
    
//    gui.addSlider("red", "r", 200, 0, 255, true);
//    gui.addSlider("green", "g", 60, 0, 255, true);
//    gui.addSlider("blue", "b", 40, 0, 255, true);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}



