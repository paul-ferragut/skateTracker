#include "particle.h"

//--------------------------------------------------------------
void particle::setup(ofVec3f startPos, float durationP,float minDrag,float maxDrag,float randomForceRange){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);


	pos = startPos;


	
	vel.x = ofRandom(-randomForceRange, randomForceRange);
	vel.y = ofRandom(-randomForceRange, randomForceRange);
	vel.z = ofRandom(-randomForceRange, randomForceRange);


	frc = ofPoint(0, 0, 0);


		drag = ofRandom(ofRandom(minDrag,maxDrag), maxDrag);//drag = ofRandom(0.95, 0.998);
		//vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
		
		duration = durationP*ofRandom(0.8,1.2);
}
void particle::addFrc(ofVec3f aFrc) {
	additionalFrc = aFrc;
}


void particle::update(ofVec3f attractionPoint,float frcIntensity)
{



		if(trail.size()<duration){
		//ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractionPoint - pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point 
	
		frc += (additionalFrc*ofMap(trail.size(), duration*ofRandom(0.8,0.99), duration,0.0,1.0,true));

		vel *= drag; //apply drag
		vel += (frc * frcIntensity); //apply force

		inc++;

		//if(inc%2==0){
		trail.push_back(pos);
		//}

		//2 - UPDATE OUR POSITION

	

		pos += vel;
		}

}

//--------------------------------------------------------------
void particle::draw(float smooth){
	line.clear();
	ofSetColor(0, 200, 200);
	ofDrawSphere(pos, 0.01);

	ofSetColor(240, 200, 200);

	line.addVertices(trail);
	//line.getSmoothed(3);
	line = line.getSmoothed(smooth);
	line.draw();
}
