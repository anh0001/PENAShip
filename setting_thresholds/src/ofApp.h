#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxChart.h"
#include "lib/ofxChartSeriesBarRange.h"

#define USE_CAMERA

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	#ifdef USE_CAMERA
		//Camera
        ofVideoGrabber vid_grabber;
        int cam_width;
        int cam_height;

	#endif

        // Opencv
        ofImage im_input;
        cv::Mat mat_input, mat_output, hsv_im;
        cv::Mat hsv[3];
        cv::Mat hue_hist, sat_hist, val_hist;

        // GUI
        ofxPanel gui;
        ofxGuiGroup group_hue;
        ofxIntSlider min_hue;
        ofxIntSlider max_hue;
        ofxGuiGroup group_saturation;
        ofxIntSlider min_saturation;
        ofxIntSlider max_saturation;
        ofxGuiGroup group_value;
        ofxIntSlider min_value;
        ofxIntSlider max_value;
        ofxGuiGroup group_ROI;
        ofxIntSlider roi_x;
        ofxIntSlider roi_y;
        ofxIntSlider roi_width;
        ofxIntSlider roi_height;

        // Histogram plots
        ofxChart::ofxChart chart;
        ofxChartAxisSetFloat *hue_axis;
        ofxChartSeriesBarRange2d<float, float> hue_plot;
        ofxChartAxisSetFloat *sat_axis;
        ofxChartSeriesBarRange2d<float, float> sat_plot;
        ofxChartAxisSetFloat *val_axis;
        ofxChartSeriesBarRange2d<float, float> val_plot;
        const int hist_size = 256;

        bool key_is_down[255];
};
