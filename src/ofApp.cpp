#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableDepthTest();

    cam.setPosition(200, -40, 900); //camera default position
    cam.lookAt(glm::vec3(0,0,0)); //look at origin

    light.setup();
    light.setPosition(200, 200, 300); //set up light position
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    light.enable();

    ofSetColor(70, 120, 60);
    ofPushMatrix();
    ofTranslate(0, -50, 0);
    ofRotateDeg(-90, 1, 0, 0); //makes the plane flat as hell
    ofDrawPlane(0, 0, 0, 800, 800);
    ofPopMatrix();

    // tree trunk base
    ofSetColor(100, 70, 40); // brown
    ofPushMatrix();
    ofTranslate(0, 50, 0); // move to ground level
    ofDrawCylinder(0, 0, 0, 30, 200); // x,y,z, radius, height
    ofPopMatrix();

    light.disable();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
