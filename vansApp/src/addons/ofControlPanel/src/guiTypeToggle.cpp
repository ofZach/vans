#include "guiTypeToggle.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

//------------------------------------------------
void guiTypeToggle::setup(string toggleName, string xmlNameIn, bool defaultVal){
	value.addValue( (int)defaultVal, 0, 1);
	name = toggleName;
	xmlName = xmlNameIn;
	
	setDimensions(14, 14);
}

//-----------------------------------------------
void guiTypeToggle::updateValue(){
	//CB
	notify();
}

//-----------------------------------------------.
void guiTypeToggle::updateGui(float x, float y, bool firstHit, bool isRelative){
	if(!firstHit)return;

	if( state == SG_STATE_SELECTED ){
		if( value.getValueI() == 0 ){
			value.setValue(1);
		}else{
			value.setValue(0);
		}

		//CB
		notify();
	}
}

//-----------------------------------------------
void guiTypeToggle::notify(){
	guiCallbackData cbVal;
	cbVal.setup(xmlName, name);
	cbVal.addValueF(value.getValueI());
	ofNotifyEvent(guiEvent,cbVal,this);
	//CB
}

//---------------------------------------------
void guiTypeToggle::updateBoundingBox(){
	hitArea.y = boundingBox.y;
}

//-----------------------------------------------.
void guiTypeToggle::render(){
	ofPushStyle();
		glPushMatrix();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			if(value.getValueI()){
				ofFill();
				glColor4fv(fgColor.getColorF());
				ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			}
			
			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			
			if(bShowText){
				glColor4fv(textColor.getColorF());
				displayText.renderText(hitArea.x + hitArea.width + 2, hitArea.y + displayText.getTextSingleLineHeight() - 2);
			}

		glPopMatrix();
	ofPopStyle();
}

void guiTypeToggle::updateText()
{
	displayText.setText( name ); 
	labelWidth = displayText.getTextWidth();
	updateBoundingBox(); 
}
