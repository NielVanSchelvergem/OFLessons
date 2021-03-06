#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    panel.setup();
    
    panel.add(noiseXSampleScale.setup("noiseXSampleScale",0.001f , 0.0001f, 0.1f));
    panel.add(noiseYSampleScale.setup("noiseYSampleScale",0.00001f , 0.000001f, 0.0001f));

    panel.add(attractionForceIn.setup("attractionForceIn",0.0001f , 0.0001f, 0.004f));
    panel.add(attractionForceOut.setup("attractionForceOut",0.0001f , 0.0001f, 0.004f));

    
    panel.add(forceRadiusIn.setup("forceRadiusIn", 100 , 0, 800));
    panel.add(forceRadiusOut.setup("forceRadiusOut",80 , 0, 800));

    panel.add(minSpeed.setup("minSpeed",3 , 0 , 40));
    panel.add(maxSpeed.setup("maxSpeed",13 , 0 , 40));
    panel.add(speedNoiseFactor.setup("speedNoiseFactor", 3 , 0 , 20));

    
    //ofSetFullscreen();
   // ofHideCursor();
    
    for(int i =0; i < 3500; i++){

        Particle p;
        p.setup();
        setParticleStartPosition(p.position);
        setParticleStartDirection(p.direction);

        particles.push_back(p);
        
    }
}


void ofApp::setParticleStartPosition(ofVec2f& incomingPosition){

    float xRandom = ofRandom(-ofGetWindowWidth(), 0);
    float yRandom = ofRandom(500, ofGetHeight() - 500);
    
    incomingPosition.set(xRandom,yRandom);
}


void ofApp::setParticleStartDirection(ofVec2f& incomingDirection){

    float xDirRandom = ofRandom(0.7,1.0);
    float yDirRandom = ofRandom(-0.1,0.1);
    
    incomingDirection.set(xDirRandom,yDirRandom);
}



//--------------------------------------------------------------
void ofApp::update(){

    
    for(int i=0; i < particles.size(); i++){
        
        
 
        addSpeedNoise(particles[i]);
        addDirectionNoise(particles[i]);

        
        
        // ADDITIONAL FORCES

     //   ofVec2f mousePos();

        
        forces.clear();
        forces.push_back(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        forces.push_back(ofVec2f(ofGetMouseY(),ofGetMouseX()));

        
        
        for(auto forcePosition : forces){
                float distance = forcePosition.distance(particles[i].position);

                float force = ofMap(distance, 0, forceRadiusOut, attractionForceOut, 0.0, true);
                ofVec2f wantedDirection = -(forcePosition - particles[i].position);
            
                float interpolationFactor = force;
                particles[i].direction += (wantedDirection - particles[i].direction) * interpolationFactor;
            
            
                float forceIn = ofMap(distance, 0, forceRadiusIn, attractionForceIn, 0.0, true);
                interpolationFactor = forceIn;
            
                particles[i].direction += -(wantedDirection - particles[i].direction) * interpolationFactor;

        }
        
        
        resetHorizontal(particles[i]);
        

        particles[i].direction.normalize();

        particles[i].update();
        
      
    }
    
}


void ofApp::resetHorizontal(Particle& p){
    // reset the particle when outsides the screen.
    if(p.position.x > ofGetWindowWidth()  || p.position.y > ofGetWindowHeight() + 200 || p.position.y < -200 )
    {
        setParticleStartPosition(p.position);
        setParticleStartDirection(p.direction);
    }
}

void ofApp::addDirectionNoise(Particle& p){
    // NOISE FORCES
    // decide where to sample in the 2D noisemap
    // by multiplying the particle x and y we stretch the shape.
    // by adding the time to the x postion we slide horizontally through the noisemap.
    
    float noiseSamplePositionX = p.position.x * noiseXSampleScale + (ofGetElapsedTimef() * 0.35);
    float noiseSamplePositionY = p.position.y * noiseYSampleScale; // + (ofGetElapsedTimef() * 0.001);
    
    // signed noise will give us a result between -1 and 1
    float noiseYDirection = ofSignedNoise(noiseSamplePositionX , noiseSamplePositionY);
    noiseYDirection *= 0.88;
    
    // bending the current y direction to the new calculated noiseResult.
    // the interpolation factor decides how much we take from new direction.
    float interpolationFactor = 0.023;
    p.direction.y += (noiseYDirection - p.direction.y) * interpolationFactor;

}


void ofApp::addSpeedNoise(Particle& p){
    
    // now we change the speed
    
    float noiseSamplePositionX = p.position.x * noiseXSampleScale + (ofGetElapsedTimef() * 0.35);
    float noiseSamplePositionY = p.position.y * noiseYSampleScale; // + (ofGetElapsedTimef() * 0.001);

    
    float newSpeed = ofNoise(noiseSamplePositionX * 2.2 , noiseSamplePositionY * 4.8) * speedNoiseFactor;
    // we use a pow to make the fast ones faster.
    newSpeed = pow(newSpeed,2.7);
    // add a minimum speed
    newSpeed = ofClamp(newSpeed, minSpeed, maxSpeed);
    
    // the interpolation factor decides how much we take from new direction.
    float interpolationFactor = 0.01;
    // we calculate the difference between the wanted and current speed.
    // than use the the interpolationFactor to decide how much we take from the difference
    p.speed += (newSpeed - p.speed) * interpolationFactor;
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofClear(2,0,23);
    
    for(int i=0; i < particles.size(); i++){
        particles[i].draw();
    }
    
    // using a circle as mousecursor
    
    ofNoFill();
    for(auto f : forces){
        ofSetColor(255, 255, 255,100);
        ofDrawCircle(f, forceRadiusOut);
        ofDrawCircle(f, forceRadiusIn);

    }
    panel.draw();
    
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

