#include "ofApp.h"

// make a willow-drooping branch spine
vector<glm::vec3> makeWillowBranchSpine(glm::vec3 start, glm::vec3 dir, float length, int points, float droop) {
    vector<glm::vec3> spine;
    glm::vec3 pos = start;
    glm::vec3 currDir = dir;
    for (int i = 0; i < points; ++i) {
        float t = i / float(points - 1);
        // curve downward more as we move out
        glm::vec3 gravity = glm::vec3(0, -1, 0);
        currDir = glm::normalize(glm::mix(dir, gravity, t * droop));
        // small noise for natural look
        glm::vec3 offset(ofNoise(i*0.2, 0) - 0.5, 0, ofNoise(i*0.2, 100) - 0.5);
        offset *= 2.0 * (1.0 - t);
        pos += currDir * length / (points - 1) + offset;
        spine.push_back(pos);
    }
    return spine;
}

void ofApp::buildBranchMesh(Branch& branch, int sides) {
    branch.mesh.clear();
    branch.mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int N = branch.spine.size();

    for(int i=0; i<N; ++i){
        float r = ofLerp(branch.baseRadius, 2.0, i/(float)(N-1));
        for(int j=0; j<sides; ++j){
            float theta = j * TWO_PI / sides;
            float x = cos(theta) * r;
            float z = sin(theta) * r;
            glm::vec3 up = (i < N-1) ? glm::normalize(branch.spine[i+1]-branch.spine[i]) : glm::vec3(0,1,0);
            glm::vec3 right = glm::normalize(glm::cross(up, glm::vec3(1,0,0)));
            glm::vec3 forward = glm::normalize(glm::cross(right, up));
            glm::vec3 pos = branch.spine[i] + right * x + forward * z;
            branch.mesh.addVertex(pos);
            branch.mesh.addColor(ofFloatColor(139/255., 69/255., 19/255.)); // brown
        }
    }
    // connect faces
    for(int i=0; i<N-1; ++i){
        for(int j=0; j<sides; ++j){
            int idx0 = i * sides + j;
            int idx1 = i * sides + (j+1)%sides;
            int idx2 = (i+1)*sides + (j+1)%sides;
            int idx3 = (i+1)*sides + j;
            branch.mesh.addIndex(idx0); branch.mesh.addIndex(idx1); branch.mesh.addIndex(idx2);
            branch.mesh.addIndex(idx0); branch.mesh.addIndex(idx2); branch.mesh.addIndex(idx3);
        }
    }
}

// Recursive branch generation
void ofApp::generateBranch(Branch& branch, glm::vec3 start, glm::vec3 dir, float length, float baseRadius, int depth) {
    int spinePts = ofMap(depth, 0, 4, 32, 7, true);
    float droop = ofMap(depth, 0, 4, 0.5, 1.2);
    branch.spine = makeWillowBranchSpine(start, dir, length, spinePts, droop);
    branch.baseRadius = baseRadius;
    branch.length = length;
    branch.depth = depth;
    buildBranchMesh(branch);

    if(depth < 4) {
        int nChild = (depth == 0) ? ofRandom(6, 9) : ofRandom(2, 4-depth);
        for(int c = 0; c < nChild; ++c){
            float minT = (depth == 0) ? 0.6 : 0.5;
            int idx = ofMap(c, 0, nChild-1, spinePts*minT, spinePts-3, true);
            glm::vec3 childStart = branch.spine[idx];

            float theta;
            if(depth == 0) {
                float baseAngle = (TWO_PI * c) / nChild;
                float jitter = ofRandom(-PI / nChild, PI / nChild);
                theta = baseAngle + jitter;
            } else {
                theta = ofRandom(-PI, PI);
            }
            glm::vec3 outward = glm::normalize(glm::vec3(cos(theta), 0.3, sin(theta)));

            glm::vec3 parentDir = glm::normalize(branch.spine[idx] - branch.spine[std::max(0, idx-2)]);
            float mixAmt = (depth == 0) ? 0.6f : 0.3f;
            glm::vec3 childDir = glm::normalize(glm::mix(parentDir, outward, mixAmt));
            if(depth > 1) childDir.y -= ofRandom(0.2, 0.5);

            Branch child;
            generateBranch(child, childStart, childDir, length * ofRandom(0.5, 0.7), baseRadius * 0.5, depth+1);
            branch.children.push_back(child);
        }
    }
}

void ofApp::drawBranch(const Branch& branch){
    branch.mesh.draw();
    for(const auto& child : branch.children){
        drawBranch(child);
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSeedRandom(900000000000); // chose this seed for a nice tree
    ofEnableDepthTest();

    // setup camera and light
    cam.setPosition(200, -40, 900);
    cam.lookAt(glm::vec3(0,0,0));
    light.setup();
    light.setPosition(200, 200, 300);

    // build the tree
    trunk.children.clear();
    glm::vec3 trunkStart(0, 0, 0);
    glm::vec3 trunkDir(0, 1, 0); // Upward
    float trunkLen = 220;
    float trunkRad = 32;
    generateBranch(trunk, trunkStart, trunkDir, trunkLen, trunkRad, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    light.enable();

    // ground plane
    ofSetColor(70, 120, 60);
    ofPushMatrix();
    ofTranslate(0, -50, 0);
    ofRotateDeg(-90, 1, 0, 0); //makes the plane flat as hell
    ofDrawPlane(0, 0, 0, 800, 800);
    ofPopMatrix();

     // Find minimum Y of trunk mesh to place on ground
    float minY = trunk.spine.empty() ? 0 : trunk.spine[0].y;
    for(const auto& v : trunk.spine) minY = std::min(minY, v.y);

    float desiredGroundY = -50; // where your ground plane is
    float offsetY = desiredGroundY - minY;

    // tree trunk base
    ofPushMatrix();
    ofTranslate(0, offsetY, 0); // move to ground level
    drawBranch(trunk);
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
