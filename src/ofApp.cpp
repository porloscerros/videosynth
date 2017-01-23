#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "Video Synth" );
    ofSetWindowShape(1280,720);
    ofSetFrameRate(60);
    ofSetBackgroundColor(255,255,255);

    //fbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    fbo2.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    //fbo3d.allocate( ofGetWidth(), ofGetHeight(), GL_RGBA );

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

    mixerGroup.setup( "Mixer" );
    mixerGroup.setHeaderBackgroundColor( ofColor::darkRed );
    mixerGroup.setBorderColor( ofColor::darkRed );
    mixerGroup.add( imageAlpha.setup( "image", 100,0,255 ));
    mixerGroup.add( videoAlpha.setup( "video", 200,0,255 ));
    mixerGroup.add( cameraAlpha.setup( "camera", 100,0,255 ));

    shader.load( "kaleido.vert", "kaleido.frag" );

    mixerGroup.add( kenabled.setup( "kenabled", true ) );
    mixerGroup.add( ksectors.setup( "ksectors", 10, 1, 100 ) );
    mixerGroup.add( kangle.setup( "kangle", 0, -180, 180 ) );
    mixerGroup.add( kx.setup( "kx", 0.5, 0, 1 ) );
    mixerGroup.add( ky.setup( "ky", 0.5, 0, 1 ) );

    mixerGroup.add( show2d.setup("show2d", 255, 0, 255) );
    mixerGroup.add( show3d.setup("show3d", 255, 0, 255) );
    mixerGroup.add( rad.setup("rad", 250, 0, 500) );
    mixerGroup.add( deform.setup("deform", 0.3, 0, 1.5) );
    mixerGroup.add( deformFreq.setup("deformFreq", 3, 0, 10) );
    mixerGroup.add( extrude.setup("extrude", 1, 0, 1 ) );


    gui.minimizeAll();
    gui.add( &mixerGroup );

    gui.loadFromFile( "settings.xml");

    ofLoadImage(image, "Pacman-cabecera.png");

    video.load( "20160113_145431.mp4" );
    video.play();

    sphere.set( 250, 80);
    vertices0 = sphere.getMesh().getVertices();
    float w = fbo2.getWidth();
    float h = fbo2.getHeight();
    sphere.mapTexCoords(0, h, w, 0);
    sphere.rotate(180, 0, 1, 0);




}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if ( camera.isInitialized() ) camera.update();

    vector<ofPoint> &vertices = sphere.getMesh().getVertices();
    for (int i=0; i<vertices.size(); i++) {
    ofPoint v = vertices0[i];
    v.normalize();
    float sx = sin( v.x * deformFreq );
    float sy = sin( v.y * deformFreq );
    float sz = sin( v.z * deformFreq );
    v.x += sy * sz * deform;
    v.y += sx * sz * deform;
    v.z += sx * sy * deform;
    v *= rad;
    vertices[i] = v;
    }

    ofPixels pixels;
    fbo2.readToPixels(pixels);
    for (int i=0; i<vertices.size(); i++) {
    ofVec2f t = sphere.getMesh().getTexCoords()[i];
    t.x = ofClamp( t.x, 0, pixels.getWidth()-1 );
    t.y = ofClamp( t.y, 0, pixels.getHeight()-1 );
    float br = pixels.getColor(t.x, t.y).getBrightness();
    vertices[i] *= 1 + br / 255.0 * extrude;
    }

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
void ofApp::draw2d(){
    ofBackground( Background );

    ofDisableSmoothing();
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofSetColor( 255, imageAlpha );
    image.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    ofSetColor( 255, videoAlpha );
    video.draw (0, 0, ofGetWidth(), ofGetHeight() );
    if ( camera.isInitialized() ) {
        ofSetColor( 255, cameraAlpha );
        camera.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    }
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    float Scl = pow( Scale, 4.0f );
    ofScale( Scl, Scl );
    ofRotate( Rotate );
    //stripePattern();
    matrixPattern();
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::draw3d(){
    fbo2.getTexture().bind();

    //light.setPosition(ofGetWidth()/2, ofGetHeight()/2, 200);
    //light.enable();
    //light.draw();
    material.begin();
    ofEnableDepthTest();

    float time = ofGetElapsedTimef();
    float longitude = 10*time;
    float latitude = 10*sin(time*0.8);
    float radius = 600 + 50*sin(time*0.4);

    cam.begin();
    cam.orbit ( longitude, latitude, radius, ofPoint(0,0,0) );
    //cam.setPosition(0,0,600);
    light.setParent(cam);
    light.enable();
    ofSetColor( ofColor::white );
    sphere.draw();

    cam.end();

    ofDisableDepthTest();
    material.end();
    light.disable();
    ofDisableLighting();

    fbo2.getTexture().unbind();

}

//--------------------------------------------------------------
void ofApp::draw(){

    fbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    fbo.begin();
    draw2d();
    fbo.end();
    //fbo.draw( 0, 0 );

    fbo2.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    fbo2.begin();
    if ( kenabled ) {
        shader.begin();
        shader.setUniform1i( "ksectors", ksectors );
        shader.setUniform1f( "kangleRad", ofDegToRad(kangle) );
        shader.setUniform2f( "kcenter", kx*ofGetWidth(), ky*ofGetHeight() );
        shader.setUniform2f( "screenCenter", 0.5*ofGetWidth(), 0.5*ofGetHeight() );
    }
    ofSetColor( 255 );
    fbo.draw( 0, 0 );
    if ( kenabled ) shader.end();
    fbo2.end();
    //fbo2.draw( 0, 0 );

    fbo3d.allocate( ofGetWidth(), ofGetHeight(), GL_RGBA );
    fbo3d.begin();
    ofBackground( 0, 0 );
    draw3d();
    fbo3d.end();

    ofBackground( 0 );
    ofSetColor( 255, show2d );
    fbo2.draw( 0, 0 );
    ofSetColor( 255, show3d );
    fbo3d.draw( 0, 0 );

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
    //if ( showGui && x < 250 ) cam.disableMouseInput();
    //else cam.enableMouseInput();

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
