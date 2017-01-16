#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

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

        void stripePattern();
        void matrixPattern();
        void draw2d();

        bool showGui;

        ofxPanel gui;
        ofxIntSlider countX;
        ofxFloatSlider stepX;
        ofxFloatSlider twistX;

        ofxGuiGroup globalGroup;
        ofxFloatSlider Scale;
        ofxFloatSlider Rotate;
        ofxFloatSlider Background;

        ofxGuiGroup primGroup;
        ofxFloatSlider shiftY, rotate;
        ofxVec2Slider size;
        ofxColorSlider color;
        ofxToggle filled, type;

        ofxIntSlider countY;
        ofxFloatSlider stepY, twistY, pinchY;

        ofTexture image;
        ofVideoPlayer video;
        ofVideoGrabber camera;

        ofxGuiGroup mixerGroup;
        ofxFloatSlider imageAlpha, videoAlpha, cameraAlpha;

        ofFbo fbo;
        ofShader shader;
        ofxToggle kenabled;
        ofxIntSlider ksectors;
        ofxFloatSlider kangle, kx, ky;


		
};
