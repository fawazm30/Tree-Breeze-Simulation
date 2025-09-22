#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableDepthTest();

    cam.setPosition(200, -40, 900); //camera default position
    cam.lookAt(glm::vec3(0,0,0)); //look at origin

    light.setup();
    light.setPosition(200, 200, 300); //set up light position

    // Build a simple tree trunk spine with some noise-based bending
    float trunkHeight = 200;
    float baseRadius = 30;
    int spinePts = 32;
    int sides = 20;

    vector<glm::vec3> spine;
    for (int i = 0; i < spinePts; i++) {
        float t = i / float(spinePts - 1);
        float y = t * trunkHeight; // start at ground level
        float bendX = ofNoise(t * 1.5 + 100) * 40 - 20; // add more bend
        float bendZ = ofNoise(t * 1.5 + 200) * 30 - 15;
        spine.push_back(glm::vec3(bendX, y, bendZ));
    }

    trunkMesh.clear();
    trunkMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

     for (int i = 0; i < spinePts; i++) {
        float r = ofLerp(baseRadius, 7, i/(float)(spinePts-1));
        for (int j = 0; j < sides; j++) {
            float theta = j * TWO_PI / sides;
            float x = cos(theta) * r;
            float z = sin(theta) * r;
            glm::vec3 up = (i < spinePts - 1) ? glm::normalize(spine[i+1] - spine[i]) : glm::vec3(0,1,0);
            glm::vec3 right = glm::normalize(glm::cross(up, glm::vec3(1,0,0)));
            glm::vec3 forward = glm::normalize(glm::cross(right, up));
            glm::vec3 pos = spine[i] + right * x + forward * z;
            trunkMesh.addVertex(pos);
            trunkMesh.addColor(ofFloatColor(139/255., 69/255., 19/255.)); // brown
        }
    }

    for (int i = 0; i < spinePts - 1; i++) {
        for (int j = 0; j < sides; j++) {
            int idx0 = i * sides + j;
            int idx1 = i * sides + (j+1)%sides;
            int idx2 = (i+1) * sides + (j+1)%sides;
            int idx3 = (i+1) * sides + j;
            // Two triangles per quad
            trunkMesh.addIndex(idx0); trunkMesh.addIndex(idx1); trunkMesh.addIndex(idx2);
            trunkMesh.addIndex(idx0); trunkMesh.addIndex(idx2); trunkMesh.addIndex(idx3);
        }
    }

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
    ofPushMatrix();
    ofTranslate(0, -50, 0); // move to ground level
    trunkMesh.draw();
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
