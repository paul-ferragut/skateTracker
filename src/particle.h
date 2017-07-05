#pragma once

#include "ofMain.h"


class particle{

	public:
		particle(){};
		~particle(){};
		void setup(ofVec3f startPos, float durationP, float minDrag, float maxDrag, float randomForceRange);
		void update(ofVec3f attractionPoint, float frcIntensity);
		void addFrc(ofVec3f aFrc);
		void draw(float smooth);

		ofVec3f pos;
		ofVec3f frc;
		ofVec3f vel;

		ofVec3f additionalFrc;

		vector<ofVec3f>trail;
		int duration;

		float drag;

		float uniqueVal;

		int inc;

		ofPolyline line;
};
