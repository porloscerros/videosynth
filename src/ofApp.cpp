#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "Video Synth" );
    ofSetWindowShape(1280,720);
    ofSetFrameRate(60);
    ofSetBackgroundColor(255,255,255);

    showGui = true;

    gui.setup( "Parameters", "settings.xml" );
    gui.add( countX.setup( "countX", 50, 0, 200));
    gui.add( stepX.setup( "stepX", 20, 0, 200));
    gui.add( twistX.setup( "twistX", 5, -45, 45));
    gui.add( countY.setup( "countY", 0, 0, 50));
    gui.add( stepY.setup( "stepY", 20, 0, 200));
    gui.add( twistY.setup( "twistY", 0, -30, 30 ));
    gui.add( pinchY.setup( "pinchY", 0, 0, 1 ));


    globalGroup.setup( "Global" );
    globalGroup.add( Scale.setup( "Scale", 1, 0.0, 1 ));
    globalGroup.add( Rotate.setup( "Rotate", 0, -180, 180 ));
    globalGroup.add( Background.setup( "Background", 255, 0, 255 ));
    gui.add( &globalGroup );

    primGroup.setup( "Primitive" );
    primGroup.add( shiftY.setup( "shiftY", 0.0, -1000.0, 1000.0 ));
    primGroup.add( rotate.setup( "rotate", 0.0, -180.0, 180.0 ));
    primGroup.add( size.setup( "size", ofVec2f(6,6), ofVec2f(0,0), ofVec2f(20,20) ));
    primGroup.add( color.setup( "color", ofColor(0), ofColor(0, 0, 0, 0), ofColor(255) ));
    primGroup.add( filled.setup( "filled", false));
    primGroup.add( type.setup( "type", false));
    gui.add( &primGroup);

    gui.loadFromFile( "settings.xml");

    ofLoadImage(image, "Pacman-cabecera.png");

    video.loadMovie( "sintom.mp4" );
    video.play();


}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if ( camera.isInitialized() ) camera.update();

}

//--------------------------------------------------------------
void ofApp::stripePattern(){
    ofSetColor( color );
    ofSetLineWidth(1);
    if ( filled ) ofFill(); else ofNoFill();
    for (int i =-countX; i<=countX; i++) {
        ofPushMatrix();
        ofTranslate( i*stepX, 0 );
        ofRotate( i*twistX );

        ofTranslate(0, shiftY);
        ofRotate( rotate );
        ofScale( size->x, size->y );
        if ( type ) ofDrawRectangle(-50, -50, 100, 100 );
        else ofDrawTriangle( 0,0,-50,100,50,100 );

        ofPopMatrix();
    }
    //ofScale( 6, 6 );
    //ofTriangle( 0,0,-50,100,50,100 );
}

//--------------------------------------------------------------
void ofApp::matrixPattern(){
    for (int y=-countY; y<=countY; y++) {
        ofPushMatrix();

        if ( countY > 0 ) {
            float scl = ofMap( y, -countY, countY, 1-pinchY, 1);
            ofScale( scl, scl);
        }
        ofTranslate( 0, y * stepY);
        ofRotate( y * twistY );
        stripePattern();

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( Background );
    ofSetColor( 255 );
    image.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    ofSetColor( 255 );
    video.draw (0, 0, ofGetWidth(), ofGetHeight() );
    if ( camera.isInitialized() ) {
        ofSetColor( 255 );
        camera.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    }

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    float Scl = pow( Scale, 4.0f );
    ofScale( Scl, Scl );
    ofRotate( Rotate );

    //stripePattern();
    matrixPattern();

    ofPopMatrix();

    if (showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'z' ) showGui = !showGui;
    if( key == OF_KEY_RETURN ) { ofSaveScreen( "screenshot.png" ); }
    if( key == 's' ) {
        ofFileDialogResult res;
        res = ofSystemSaveDialog( "preset.xml", "Saving Preset");
        if( res.bSuccess ) gui.saveToFile ( res.filePath );
    }
    if( key == 'l' ) {
        ofFileDialogResult res;
        res = ofSystemLoadDialog ( "Loading Preset" );
        if( res.bSuccess ) gui.loadFromFile ( res.filePath );
    }
    if( key == 'c' ) {
        camera.setDeviceID( 0 );
        camera.setDesiredFrameRate( 30 );
        camera.initGrabber( 1280, 720 );
    }

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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile( "settings.xml");

}
