#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	prevTime = 0;
	newTime = 0;
	radiusCm = 4.89;
	wheelCircumference = radiusCm*TWO_PI;


	directionOverall.set(0, 0, 0);



	filename = "LOG60inwardheel61";
	ofBuffer buffer = ofBufferFromFile(filename+".TXT");//"LOG11.TXT" //"LOG14.TXT"
	cout << "buffer" << endl;
	for (auto line : buffer.getLines()) {
		linesOfTheFile.push_back(line);
	}
	//cout << "lines" << linesOfTheFile.size()<<endl;
	linesOfTheFile.erase(linesOfTheFile.begin());
	linesOfTheFile.pop_back();
	linesOfTheFile.pop_back();

	for (int i = 0;i < linesOfTheFile.size();i++) {

		string currentLine = linesOfTheFile[i];
		//cout << "current line" << currentLine << endl;
		vector<string>parsedArguments = ofSplitString(currentLine, ";", true, true);


		for (int j = 0;j < parsedArguments.size();j++) {
			vector<string>valueString = ofSplitString(parsedArguments[j], ":", true, true);
			string line1 = valueString[0];
			string line2 = valueString[1];

			if (line1 == "qW") {
				vec4T.w = ofToFloat(line2);
			}
			else if (line1 == "qX") {
				vec4T.x = ofToFloat(line2);
			}
			else if (line1 == "qY") {
				vec4T.z = ofToFloat(line2);
			}
			else if (line1 == "qZ") {
				vec4T.y = ofToFloat(line2);
			}
			else if (line1 == "hall") {
				hallSensorT = ofToBool(line2);
			}
			else if (line1 == "altitude") {
				heightT = ofToFloat(line2);
			}
			else if (line1 == "time") {
				timeMillisT = ofToFloat(line2);
			}

			quatT.set(vec4T);
			quat.push_back(quatT);
			hallSensor.push_back(hallSensorT);
			height.push_back(heightT);
			timeMillis.push_back(timeMillisT);

		}
	}
	cout << "calculateSpeed " << endl;
	calculateSpeed();
	cout << "calculateHeight " << endl;


	trickStart = (float)timeMillis.size()*0.4;
	trickEnd = (float)timeMillis.size()*0.6;

	rollBegin = (float)timeMillis.size()*0.2;
	rollEnd = (float)timeMillis.size()*0.8;

	calculateHeight();
	cout << "calculatePts " << endl;


	tailLeftWheel= calculatePts(0.1, ofVec3f(0,0,-9));
	tailRightWheel= calculatePts(0.1, ofVec3f(0, 0, +9));
	noseLeftWheel= calculatePts(0.1, ofVec3f(32, 0, -9));
	noseRightWheel= calculatePts(0.1, ofVec3f(32, 0, +9));

	cout << "mesh loaded " << endl;
	gui.setup("settings");
	gui.add(timeCursor.setup("time cursor", 0, 0, timeMillis.size() - 1));
	gui.add(playPause.setup("play Pause", false));
	
	gui.add(useParticles.setup("use Particles", false));
	gui.add(resetTrail.setup("resetTrail", false));
	
	gui.add(trickStart.setup("trickStart", 0, timeMillis.size()*0.2, timeMillis.size()*0.8 - 1));
	gui.add(trickEnd.setup("trickEnd", 0, timeMillis.size()*0.2, timeMillis.size()*0.8 - 1));

	gui.add(rollBegin.setup("rollBegin", 0, 0, timeMillis.size() - 1));
	gui.add(rollEnd.setup("rollEnd", 0, 0, timeMillis.size() - 1));

	gui.add(flipDirection.setup("flipDirection", 0, 0, timeMillis.size() - 1));

	gui.add(smoothing.setup("smoothing", 10,0,30));
	gui.add(estimationHeight.setup("estimationHeight", 50, 0, 100));

	gui.add(sizeTube.setup("sizeTube", 1, 0, 8));
	gui.add(trailToMesh.setup("trail To Mesh", false));
	gui.add(saveMesh.setup("save mesh", false));

	gui.add(resetParticles.setup("resetParticles", false));
	gui.add(dampingMin.setup("dampingMin", 1, 0.8, 0.999));
	gui.add(dampingMax.setup("dampingMax", 1, 0.8, 0.999));
	gui.add(randomForce.setup("randomForce", 1, 0, 4.0));
	gui.add(durationParticles.setup("durationParticles", 50, 20, 200));
	gui.add(attractionStrenght.setup("attractionStrenght", 0.6, 0.1, 1.0));
	gui.add(intensityWind.setup("intensityWind", 0.6, 0.0, 4.0));
	gui.add(windFrc.setup("wind frc",ofVec3f(0, 0,0), ofVec3f(-1.0, -1.0, -1.0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(everyNFrames.setup("everyNFrames", 1, 1, 30));
	gui.add(particlesToMesh.setup("particlesToMesh", false));

	gui.loadFromFile(filename+".xml");

	skateMesh.load("skate.ply");

	resetTrail = true;

}

//--------------------------------------------------------------
void ofApp::update(){

	quatT = quat[timeCursor];
	distanceTravelledT = distanceTravelled[timeCursor];

	if (playPause) {
		timeCursor = timeCursor + 1;
		if (timeCursor > timeMillis.size()) {
			timeCursor = 0;
		}
	}

	if (trickStart!=trickStartChange) {
		trickStartChange = trickStart;
		resetTrail = true;
	}
	if (trickEnd != trickEndChange) {
		trickEndChange = trickEnd;
		resetTrail = true;
	}
	if (rollEnd != rollEndChange) {
		rollEndChange = rollEnd;
		resetTrail = true;
	}
	if (rollBegin != rollEndChange) {
		rollEndChange = rollBegin;
		resetTrail = true;
	}

	if (resetTrail) {
		calculateSpeed();
		calculateHeight();


		tailLeftWheel = calculatePts(0.1, ofVec3f(0, 0, -9));
		tailRightWheel = calculatePts(0.1, ofVec3f(0, 0, +9));
		noseLeftWheel = calculatePts(0.1, ofVec3f(32, 0, -9));
		noseRightWheel = calculatePts(0.1, ofVec3f(32, 0, +9));
		resetTrail = false;
	}

	if (resetParticles) {
		particlesLeftNose.clear();
		particlesLeftTail.clear();
		particlesRightNose.clear();
		particlesRightTail.clear();

		allTrails.clear();
		ptf.clear();
		meshes.clear();
		resetParticles = false;
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	ofEnableAlphaBlending();
	
	cam.begin();

	ofDrawAxis(10);
	ofPushMatrix();
	ofRotateX(90);
	ofSetColor(255, 255, 255, 20);
	ofDrawRectangle(-1000, -1000, 2000, 2000);

	ofPopMatrix();

	ofSetColor(255, 255, 255, 255);
		ofVec3f axis;
	float angle;
	quatT.getRotate(angle, axis);

	ofEnableDepthTest();
	ofPushMatrix();

	ofTranslate(directionalVec[timeCursor].x, heightComputed[timeCursor],directionalVec[timeCursor].y);//	
	ofRotate(angle,axis.x,axis.y,axis.z);	
	ofTranslate(16,0,0);//	
	
	ofDrawAxis(10);
	skateMesh.drawWireframe();
	
	ofPopMatrix();
	ofDrawAxis(20);

	ofPolyline leftTail;
	ofSetColor(255, 0, 0);
	for (int i = 0;i < tailLeftWheel.size();i++) {
		if (i > 0 && i < timeCursor) {
			leftTail.addVertex(tailLeftWheel[i]);	
		}
	}
	leftTail = leftTail.getSmoothed(smoothing);
	leftTail.draw();


	ofSetColor(255, 255, 255);
	for (int i = 0;i < tailLeftWheel.size();i++) {
		if (height[i] > 78 && i > rollBegin&&i< rollEnd) {
		ofDrawSphere(tailLeftWheel[i], 0.4);
		}
	}


	ofPolyline rightTail;
	ofSetColor(0, 255, 0);
	for (int i = 0;i < tailRightWheel.size();i++) {
		if (i > 0 && i < timeCursor) {
			rightTail.addVertex(tailRightWheel[i]);

		}
	}
	rightTail = rightTail.getSmoothed(smoothing);
	rightTail.draw();

	ofPolyline leftNose;
	ofSetColor(0, 0, 255);
	for (int i = 0;i < noseLeftWheel.size();i++) {
		if (i > 0 && i < timeCursor) {
			leftNose.addVertex(noseLeftWheel[i]);
			

		}
	}
	leftNose = leftNose.getSmoothed(smoothing);
	leftNose.draw();

	ofPolyline rightNose;
	ofSetColor(255, 255, 0);
	for (int i = 0;i < noseRightWheel.size();i++) {
		if (i > 0 && i < timeCursor) {
			rightNose.addVertex(noseRightWheel[i]);
		}
	}
	rightNose=rightNose.getSmoothed(smoothing);
	rightNose.draw();

	ofSetColor(255, 0, 0);
	ofDrawSphere(tailLeftWheel[trickStart], 2);
	ofDrawBitmapString("Trick begin", tailLeftWheel[trickStart]);
	ofSetColor(0,255, 0);
	ofDrawSphere(tailLeftWheel[trickEnd], 2);
	ofDrawBitmapString("Trick End", tailLeftWheel[trickEnd]);

	ofSetColor(0, 255, 255);
	ofDrawSphere(tailLeftWheel[rollBegin], 2);
	ofDrawBitmapString("Roll Begin", tailLeftWheel[rollBegin]);

	ofSetColor(255, 0, 255);
	ofDrawSphere(tailLeftWheel[rollEnd], 2);
	ofDrawBitmapString("Roll End", tailLeftWheel[rollEnd]);

	if(timeCursor>150){
		ofVec3f dirRN=  rightNose.getPointAtIndexInterpolated(timeCursor)- rightNose.getPointAtIndexInterpolated(timeCursor-150);
		ofVec3f dirRT = rightTail.getPointAtIndexInterpolated(timeCursor) - rightTail.getPointAtIndexInterpolated(timeCursor - 150);
		ofVec3f dirLN = leftNose.getPointAtIndexInterpolated(timeCursor) - leftNose.getPointAtIndexInterpolated(timeCursor - 150);
		ofVec3f dirLT = (leftTail.getPointAtIndexInterpolated(timeCursor) - leftTail.getPointAtIndexInterpolated(timeCursor - 150));
		directionOverall = (dirRN + dirRT + dirLN + dirLT) / 4.0;
		directionOverall.normalize();
		directionOverall *= intensityWind;
		directionOverall *= -1;
	}
	windFrc = directionOverall;
	
	if(useParticles){
	particleTrail(rightNose, trailToMesh, particlesRightNose);
	particleTrail(rightTail,trailToMesh, particlesRightTail);
	particleTrail(leftNose,trailToMesh, particlesLeftNose);
	particleTrail(leftTail, trailToMesh, particlesLeftTail);

		if (trailToMesh) {

			ofxPtf nPtf;
			ofVboMesh meshT;
			ptf.clear();
			meshes.clear();
			for (int i = 0; i < allTrails.size(); i++) {
				ptf.push_back(nPtf);
				meshes.push_back(meshT);
			}

			for (int i = 0; i < allTrails.size(); i++) {
				ptf[i].setupPtfPoints(allTrails[i].getVertices(), 35, sizeTube, sizeTube*0.05);
				meshes[i] = ptf[i].getMesh();
			}

			trailToMesh = false;
		}

	}
	else {
		if (trailToMesh) {

			ofxPtf nPtf;
			ofVboMesh meshT;
			ptf.clear();
			meshes.clear();
			for (int i = 0; i < 4; i++) {
				ptf.push_back(nPtf);
				meshes.push_back(meshT);
			}

			ptf[0].setupPtfPoints(leftTail.getVertices(), 35, sizeTube, sizeTube);
			meshes[0] = ptf[0].getMesh();
			ptf[1].setupPtfPoints(rightTail.getVertices(), 35, sizeTube, sizeTube);
			meshes[1] = ptf[1].getMesh();
			ptf[2].setupPtfPoints(leftNose.getVertices(), 35, sizeTube, sizeTube);
			meshes[2] = ptf[2].getMesh();
			ptf[3].setupPtfPoints(rightNose.getVertices(), 35, sizeTube, sizeTube);
			meshes[3] = ptf[3].getMesh();

			trailToMesh = false;
		}
	}


	ofSetColor(255, 255, 255);
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].drawWireframe();
	}
	cam.end();
	
	ofDisableDepthTest();

	gui.draw();

	if (saveMesh) {
		string time = ofGetTimestampString();
		for (int i = 0; i < ptf.size(); i++) {
			/*ofxCSG::meshUnion(meshes[i], vboMesh, vboMesh); 
			to do CSG for merging
			for now take all the mesh and merge in meshlab or meshmixer
			*/

			if (meshes[i].getNumVertices()>10 && meshes[i].getNumIndices()>10) {
				meshes[i].save(time +"/"+ofToString(i) + ".ply", false);
			}
		}
		//vboMesh.save(ofGetTimestampString() + ".ply", false);
		saveMesh = false;

	}
}

//--------------------------------------------------------------
void ofApp::exit() {

	gui.saveToFile(filename+".xml");

}


void ofApp::calculateSpeed() {

	speedAtTime.clear();
	state.clear();
	distanceTravelled.clear();
	directionalVec.clear();

	float incSpeed=0;
	vector<int>prevIndex;
	vector<int>nextIndex;
	int lastPrevIndex;
	int lastNextIndex;
	for (int i = 0;i < hallSensor.size();i++) {

		//FIND THE PREVIOUS AND NEXT HALL SENSOR
		int indexPrev = 0;
		int indexNext = 0;
		int inc = i;
		int dec = i;

		while (dec >= 0 && hallSensor[dec] == false) {
			dec--;
			indexPrev = dec;
		}

		while (inc <= hallSensor.size() && hallSensor[inc] == false) {
			inc++;
			indexNext = inc;
		}
		
		int incValidHallSensor = 0;
		while (incValidHallSensor <= hallSensor.size() && hallSensor[indexNext + incValidHallSensor] == true) {
			incValidHallSensor++;
		}
		indexNext += incValidHallSensor;

		if(hallSensor[i]){
			indexPrev = lastPrevIndex;
			indexNext = lastNextIndex;
		}
		else {
			lastPrevIndex = indexPrev;
			lastNextIndex = indexNext;
		}

		prevIndex.push_back(indexPrev);
		nextIndex.push_back(indexNext);

	}

	for (int i = 0;i < height.size();i++) {
		int currentState = INACTIVE;

		//SET STATES WITH HEIGHT RANGE
		if (i > rollBegin && i < rollEnd) {
			currentState = ROLL;
		}
		if (i > trickStart&& i < trickEnd) {
			currentState = TRICK;
		}
		state.push_back(currentState);
		//POPULATE VECTORS
		distanceTravelled.push_back(0.0f);
		ofVec2f v2;
		directionalVec.push_back(v2);
	}

	float lastDistance = 0;
	ofVec2f lastPostion2d;
	ofVec2f last2dPostionInc;
	float lastInc = 0;
	for (int i = 0;i < height.size();i++) {
		
		if (state[i]==ROLL) {
		
			if (prevIndex[i] >= 0 && nextIndex[i] <= hallSensor.size()) {


				int indexDiff = nextIndex[i] - prevIndex[i];
				//cout << "diff" << indexDiff << endl;
				float increment = 0;
				if (indexDiff != 0)
					increment = wheelCircumference / (float)indexDiff;

				lastDistance += increment;

				distanceTravelled[i] += lastDistance;


				lastInc = increment;


				ofVec3f axis;
				float angle;
				quat[i].getRotate(angle, axis);


				ofVec3f angles = quat[i].getEuler();
				//cout << "rad" << angle << endl;
				directionalVec[i].x = lastPostion2d.x + increment*cos(ofDegToRad(angles.y));
				directionalVec[i].y = lastPostion2d.y + increment*sin(ofDegToRad(angles.y) + PI);

				if (i > flipDirection) {
					directionalVec[i].x = lastPostion2d.x + increment*cos(ofDegToRad(angles.y)+PI);
					directionalVec[i].y = lastPostion2d.y + increment*sin(ofDegToRad(angles.y)+ PI + PI);
				}

				lastPostion2d = directionalVec[i];
				last2dPostionInc.set(increment*cos(ofDegToRad(angles.y)), increment*sin(ofDegToRad(angles.y) + PI));


			}

		}
		else if(state[i] == INACTIVE){
		
			distanceTravelled[i] = lastDistance;
			directionalVec[i]=lastPostion2d;
		}
		else if (state[i] == TRICK) {
			
			lastDistance += (lastInc);
			distanceTravelled[i] += lastDistance;
			directionalVec[i] = lastPostion2d + (last2dPostionInc*2);//MANUAL CHANGED
			lastPostion2d = directionalVec[i];

		}
	}

	for (int i = 0;i < hallSensor.size();i++) {

		//FIND THE PREVIOUS AND NEXT HALL SENSOR
		float increment = FOUR_PI / (float)(nextIndex[i] - prevIndex[i]);
		incSpeed += increment;
		speedAtTime.push_back(incSpeed);

	}

}

void ofApp::calculateHeight() {


	
	heightComputed.clear();

	vector<int>trickBeginIndex;
	vector<int>trickEndIndex;
	vector<int>trickDuration;
	int lastState = INACTIVE;
	for (int i = 0;i < height.size();i++) {
		if (state[i] != lastState) {
		
			if ((lastState == ROLL) && state[i] == TRICK) {
				trickBeginIndex.push_back(i);
			}

			if (lastState == TRICK && state[i] == ROLL) {
				trickEndIndex.push_back(i);
			}
		
		}
		lastState = state[i];

	}

	for (int i = 0;i < trickBeginIndex.size();i++) {
		trickDuration.push_back(trickEndIndex[i] - trickBeginIndex[i]);
	}

	float rollingHeight=85;
	for (int i = 0;i < height.size();i++) {
		float heightComputedT = 0;
		float lastHeight = 0;
		if (state[i] == ROLL) {
			heightComputedT = height[i] / 10;
			heightComputed.push_back(heightComputedT);
			lastHeight = heightComputedT;
			rollingHeight = lastHeight;
		}
		else if (state[i] == TRICK) {
			
			int j = 0;
			while (i >= trickBeginIndex[j] && i >= trickEndIndex[j]) {
				j++;
			}
			
			lastHeight = rollingHeight+(1.0+sin(ofMap(i, trickBeginIndex[j], trickEndIndex[j], -HALF_PI, 3 * HALF_PI, true)))*estimationHeight;
			heightComputed.push_back(lastHeight);

		}
		else {
			heightComputed.push_back(lastHeight);
		}
	}

}

vector<ofVec3f> ofApp::calculatePts(float smoothing , ofVec3f offsetPos)
{

	vector<ofVec3f>trailReturn;

	for (int i = 0;i < quat.size();i++) {
	
		ofVec3f posCenter(0,0,0);
		
		posCenter += offsetPos;
		posCenter = quat[i]*posCenter;
		posCenter+=ofVec3f(directionalVec[i].x, heightComputed[i], directionalVec[i].y);

		trailReturn.push_back(posCenter);
	
	}

	return trailReturn;
}

void ofApp::particleTrail(ofPolyline pLine, bool save, vector<particle>&p)
{


	ofVec3f currentPos;
	currentPos = pLine.getPointAtIndexInterpolated(timeCursor);
	ofFill();
	ofVec3f magnetOnWheel(0, 0, 0);
	magnetOnWheel += ofVec3f(cos((speedAtTime[timeCursor])*-2)*radiusCm / 2, sin((speedAtTime[timeCursor])*-2)*radiusCm / 2, 0);
	magnetOnWheel = quat[timeCursor] * magnetOnWheel;
	magnetOnWheel += currentPos;

	ofDrawSphere(magnetOnWheel, 0.2);

	if (playPause) {

		if (ofGetFrameNum() % everyNFrames == 0) {
			particle tP;
			tP.setup(magnetOnWheel, durationParticles, dampingMin, dampingMax, randomForce);
			p.push_back(tP);
		}
	}

	for (int i = 0;i < p.size();i++) {
		if (playPause) {
			p[i].addFrc(ofVec3f(windFrc));
			p[i].update(magnetOnWheel, attractionStrenght);
		}
	}


	ofNoFill();

	for (int i = 0;i < p.size();i++) {
		p[i].draw(smoothing);	
		if (save) {
			allTrails.push_back(p[i].line);
		}
	}



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
/*
vector<float> ofApp::calculateSpeed(vector<bool> hallS)
{
	vector<float>t;
	return t;
}*/

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
string ofApp::ofxGetSerialString(ofSerial &serial, char until) {
	static string str;
	stringstream ss;
	char ch;
	int ttl = 1000;
	while ((ch = serial.readByte())>0 && ttl-->0 && ch != until) {
		ss << ch;
	}
	str += ss.str();
	if (ch == until) {
		string tmp = str;
		str = "";
		return ofxTrimString(tmp);
	}
	else {
		return "";
	}
}
// trim trailing spaces
string ofApp::ofxTrimStringRight(string str) {
	size_t endpos = str.find_last_not_of(" \t\r\n");
	return (string::npos != endpos) ? str.substr(0, endpos + 1) : str;
}

// trim trailing spaces
string ofApp::ofxTrimStringLeft(string str) {
	size_t startpos = str.find_first_not_of(" \t\r\n");
	return (string::npos != startpos) ? str.substr(startpos) : str;
}

string ofApp::ofxTrimString(string str) {
	return ofxTrimStringLeft(ofxTrimStringRight(str));;
}