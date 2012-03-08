#include "positionableGraphic.h"
#include "ofxXmlSettings.h"

class graphicLoader{
	
	public:
	
		void setup(float w, float h){
			screenW = w;
			screenH = h;
		}
	
		void loadGraphics(string treePath, ofPoint anchor, float defaultY, bool bEvenDistrib = false){
			ofDirectory dir;
			dir.allowExt("png");
			int numFiles = dir.listDir(treePath);
			
			ofxXmlSettings xml; 
			xml.loadFile(treePath+"imageSettings.xml");
			
			for(int k = 0; k < numFiles; k++){					
				graphics.push_back( new positionableGraphic() );
				graphics.back()->img.loadImage( dir.getPath(k), false, true );
				graphics.back()->img.setAnchorPercent(anchor.x, anchor.y);
				
				float x = 0.0;
				if( bEvenDistrib ){
					x = ofMap( k, 0, numFiles, 0, screenW, true); 
				}else{
					x = ofRandom(0, screenW);
				}
				
				graphics[k]->pos.x = xml.getValue("graphic:pos:x", x, k);
				graphics[k]->pos.y = xml.getValue("graphic:pos:y", defaultY, k);
				graphics[k]->scale = xml.getValue("graphic:scale", ofRandom(0.9, 1.2), k);
			
			}
			
		}
		
		vector <positionableGraphic *> & getGraphics(){
			return graphics;
		}
		
		vector <positionableGraphic *> graphics;

		float screenW, screenH;
};