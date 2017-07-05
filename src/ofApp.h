#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include <string>
#include "ofxPtf.h"
#include "particle.h"

#define USELOG

#define INACTIVE 0
#define ROLL 1
#define TRICK 2
//1 cm = 1 pixel

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		string ofxGetSerialString(ofSerial &serial, char until);
		string ofxTrimStringRight(string str);
		string ofxTrimStringLeft(string str);
		string ofxTrimString(string str);

		vector < string > linesOfTheFile;


		vector<ofVec4f>vec4;
		vector<ofQuaternion>quat;
		vector<float>height;
		vector<float>heightComputed;
		vector<bool>hallSensor;
		vector<float>timeMillis;

		vector<float>distanceTravelled;
		vector<ofVec2f>directionalVec;
		vector<int>state;
		vector<float>speedAtTime;

		vector<ofVec3f>tailLeftWheel;
		vector<ofVec3f>tailRightWheel;
		vector<ofVec3f>noseLeftWheel;
		vector<ofVec3f>noseRightWheel;

		ofxVec3Slider dofT;
		ofxVec3Slider posT;
		float heightT;
		bool hallSensorT;
		float timeMillisT;
		ofQuaternion quatT;
		ofVec4f vec4T;
		float distanceTravelledT;
		//float heightComputed;

		ofVboMesh skateMesh;

		ofxPanel gui;

		ofxFloatSlider timeCursor;

		ofxFloatSlider smoothing;
		ofxFloatSlider estimationHeight;

		ofxFloatSlider sizeTube;


		ofxFloatSlider intensityWind;

		ofxToggle playPause;

		void calculateSpeed();
		void calculateHeight();

		ofEasyCam cam;
		ofSerial serial;

		string filename;



		float prevTime;
		float newTime;
		float radiusCm;
		float wheelCircumference;
		//bool hallSensor;
		

		vector<ofVec3f> calculatePts(float smoothing,ofVec3f offsetPos);


		vector<ofxPtf>ptf;
		vector<ofVboMesh>meshes;

		ofVboMesh vboMesh;

		//ofxToggle recordTrail;
		ofxToggle trailToMesh;
		ofxToggle saveMesh;

		
		vector<ofVec3f>magnetPt;


		ofxToggle resetParticles;
		ofxFloatSlider randomForce;
		ofxFloatSlider dampingMin;
		ofxFloatSlider dampingMax;
		ofxFloatSlider attractionStrenght;
		//ofxToggle createParticles;
		ofxVec3Slider windFrc;
		ofxIntSlider everyNFrames;

		ofxIntSlider trickStart, trickEnd,rollBegin,rollEnd;

		int trickStartChange, trickEndChange, rollBeginChange, rollEndChange;

		ofxToggle useParticles;
		ofxToggle resetTrail;
		ofxIntSlider flipDirection;
		

		ofxIntSlider durationParticles;
		ofxToggle particlesToMesh;


		vector<particle>particlesLeftNose;
		vector<particle>particlesLeftTail;
		vector<particle>particlesRightNose;
		vector<particle>particlesRightTail;

		void particleTrail(ofPolyline pLine, bool save, vector<particle>&p);



		vector<ofPolyline>allTrails;

		ofVec3f directionOverall;

};
