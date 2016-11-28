#include "ofApp.h"

#define CV_RGB2HSV 3

//--------------------------------------------------------------
void ofApp::setup(){

#ifdef USE_CAMERA
	cam_width = 320;  // try to grab at this size.
	cam_height = 240;

	//Set camera properties
	vid_grabber = cv::VideoCapture(0);
	vid_grabber.set(CV_CAP_PROP_FRAME_HEIGHT, cam_height);
	vid_grabber.set(CV_CAP_PROP_FRAME_WIDTH, cam_width);
	vid_grabber.set(CV_CAP_PROP_FPS, 30);

	if(!vid_grabber.isOpened()){
		cout << "Capture Error.. ..\n Cek Kamera Yaaa" << endl;
		//return -1;
	}else{
		cout << "HOreee.. " << endl;
		//namedWindow("PENSHIP CAM",1);
	}

	ofSetVerticalSync(true);

	vid_grabber >> mat_input;
	cv::cvtColor(mat_input, hsv_im, CV_RGB2HSV);
	mat_output = cv::Mat::zeros(mat_input.rows, mat_input.cols, CV_8UC1);

#else
	im_input.load("blue.png");// --- OpenCV ---
    mat_input = ofxCv::toCv(im_input).clone();
    cv::cvtColor(mat_input, hsv_im, CV_RGB2HSV);
    mat_output = cv::Mat::zeros(mat_input.rows, mat_input.cols, CV_8UC1);
#endif

    // --- GUI ---
    gui.setup("Thresholds", "thresholds.xml",10,340);
    gui.add(group_hue.setup("HUE Min-Max"));
    group_hue.add(min_hue.setup("minimum",50,0,255));
    group_hue.add(max_hue.setup("maximum",50,0,255));

    gui.add(group_saturation.setup("Saturation Min-Max"));
    group_saturation.add(min_saturation.setup("minimum",50,0,255));
    group_saturation.add(max_saturation.setup("maximum",50,0,255));

    gui.add(group_value.setup("Value Min-Max"));
    group_value.add(min_value.setup("minimum",50,0,255));
    group_value.add(max_value.setup("maximum",50,0,255));

    gui.add(group_ROI.setup("ROI"));
    group_ROI.add(roi_x.setup("x",50,0,mat_input.cols));
    group_ROI.add(roi_y.setup("y",50,0,mat_input.rows));
    group_ROI.add(roi_width.setup("width",50,0,mat_input.cols));
    group_ROI.add(roi_height.setup("height",50,0,mat_input.rows));

    // --- Plotting
    hue_plot.setPointContainerSize(0.98);
    sat_plot.setPointContainerSize(0.98);
    val_plot.setPointContainerSize(0.98);
    for (int i=0; i<hist_size; i++)  // Init plots
    {
    	hue_plot.addColumn(0.0f, 10.0f, ofToString(i));
    	sat_plot.addColumn(0.0f, 10.0f, ofToString(i));
    	val_plot.addColumn(0.0f, 10.0f, ofToString(i));
    }
    hue_plot.setBaseColor(0,0,255);
    sat_plot.setBaseColor(0,0,255);
    val_plot.setBaseColor(0,0,255);

    hue_axis = chart.createAxisSetXY<ofxChartDataPointFloat2d>();
    hue_axis->addDataSeries(&hue_plot);
    hue_axis->setDimensions(ofGetWidth()/3, ofGetHeight()/8);

    sat_axis = chart.createAxisSetXY<ofxChartDataPointFloat2d>();
    sat_axis->addDataSeries(&sat_plot);
    sat_axis->setDimensions(ofGetWidth()/3, ofGetHeight()/8);

    val_axis = chart.createAxisSetXY<ofxChartDataPointFloat2d>();
    val_axis->addDataSeries(&val_plot);
    val_axis->setDimensions(ofGetWidth()/3, ofGetHeight()/8);
}

//--------------------------------------------------------------
void ofApp::update(){
    cv::split(hsv_im, hsv);

    int channels[] = {0};
    float range[] = { 0, 256 };
    const float* ranges[] = { range };
    int histogram_size[] = {hist_size};

    cv::Mat mask = cv::Mat::zeros(mat_input.rows, mat_input.cols, CV_8UC1);
    cv::Mat roi = mask(cv::Rect(roi_x,roi_y,roi_width,roi_height));
    roi = 255;

    cv::calcHist(&hsv[0], 1, channels, mask, hue_hist, 1, histogram_size, ranges, true, false);
    cv::calcHist(&hsv[1], 1, channels, mask, sat_hist, 1, histogram_size, ranges, true, false);
    cv::calcHist(&hsv[2], 1, channels, mask, val_hist, 1, histogram_size, ranges, true, false);

    // cout << "rows=" << hue_hist.rows << " cols=" << hue_hist.cols << endl;

    for (int i=0; i<hue_hist.rows; i++)
    {
    	hue_plot.updateDataPointY(i, 0.0f, hue_hist.at<float>(i,0));
    	sat_plot.updateDataPointY(i, 0.0f, sat_hist.at<float>(i,0));
    	val_plot.updateDataPointY(i, 0.0f, val_hist.at<float>(i,0));
    }

    cv::Mat thresholded_hue, thresholded_sat, thresholded_val;
    cv::inRange(hsv[0], cv::Scalar(min_hue), cv::Scalar(max_hue), thresholded_hue);
    cv::inRange(hsv[1], cv::Scalar(min_saturation), cv::Scalar(max_saturation), thresholded_sat);
    cv::inRange(hsv[2], cv::Scalar(min_value), cv::Scalar(max_value), thresholded_val);
    cv::bitwise_or(thresholded_hue, thresholded_sat, mat_output, cv::Mat());
    cv::bitwise_or(mat_output, thresholded_val, mat_output, cv::Mat());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(200, 200, 200);

    int thickness = 5;
    cv::Mat im_with_rectangle = mat_input.clone();
    cv::rectangle(im_with_rectangle, cv::Rect(roi_x,roi_y,roi_width,roi_height),
    		cv::Scalar(10,255,10), thickness, 8, 0);

    // Draw input and output images
    ofxCv::drawMat(im_with_rectangle, 5,5,480,320);
    ofxCv::drawMat(mat_output, 500,5,480,320);

    ofPushMatrix();
        ofTranslate(ofGetWidth()/4, ofGetHeight()*0.58);
        ofScale(1.0f,-1.0f,1.0f);  //Flip
        hue_plot.draw();
        hue_plot.drawThresholdLines(min_hue, max_hue);
        hue_plot.lightsOn();
    ofPopMatrix();

    ofPushMatrix();
        ofTranslate(ofGetWidth()/4, ofGetHeight()*0.73);
        ofScale(1.0f,-1.0f,1.0f);  //Flip
        sat_plot.draw();
        sat_plot.drawThresholdLines(min_saturation, max_saturation);
    ofPopMatrix();

    ofPushMatrix();
    	ofTranslate(ofGetWidth()/4, ofGetHeight()*0.88);
        ofScale(1.0f,-1.0f,1.0f);  //Flip
        val_plot.draw();
        val_plot.drawThresholdLines(min_value, max_value);
    ofPopMatrix();

    // GUI draw
    gui.draw();

    // --- Keys
    float button_step = 0.2f;
    static float tot_step = 0.0f;
    tot_step += button_step;
    if (tot_step > 1.0f)
    {
    	tot_step = 0.0f;

    	if (key_is_down['1'] && key_is_down['-'])
    		min_hue = min_hue - 1;
    	else if (key_is_down['1'] && key_is_down['='])
    		min_hue = min_hue + 1;
    	if (key_is_down['2'] && key_is_down['-'])
    		max_hue = max_hue - 1;
    	else if (key_is_down['2'] && key_is_down['='])
    		max_hue = max_hue + 1;

    	if (key_is_down['3'] && key_is_down['-'])
    		min_saturation = min_saturation - 1;
    	else if (key_is_down['3'] && key_is_down['='])
    		min_saturation = min_saturation + 1;
    	if (key_is_down['4'] && key_is_down['-'])
    		max_saturation = max_saturation - 1;
    	else if (key_is_down['4'] && key_is_down['='])
    		max_saturation = max_saturation + 1;

    	if (key_is_down['5'] && key_is_down['-'])
    		min_value = min_value - 1;
    	else if (key_is_down['5'] && key_is_down['='])
    		min_value = min_value + 1;
    	if (key_is_down['6'] && key_is_down['-'])
    		max_value = max_value - 1;
    	else if (key_is_down['6'] && key_is_down['='])
    		max_value = max_value + 1;

    	if (key_is_down['7'] && key_is_down['-'])
    		roi_x = roi_x - 1;
    	else if (key_is_down['7'] && key_is_down['='])
    		roi_x = roi_x + 1;

    	if (key_is_down['8'] && key_is_down['-'])
    		roi_y = roi_y - 1;
    	else if (key_is_down['8'] && key_is_down['='])
    		roi_y = roi_y + 1;

    	if (key_is_down['9'] && key_is_down['-'])
    		roi_width = roi_width - 1;
    	else if (key_is_down['9'] && key_is_down['='])
    		roi_width = roi_width + 1;

    	if (key_is_down['0'] && key_is_down['-'])
    		roi_height = roi_height - 1;
    	else if (key_is_down['0'] && key_is_down['='])
    		roi_height = roi_height + 1;

#ifdef USE_CAMERA
    	if (key_is_down['c'])
    	{
    		vid_grabber >> mat_input;
    		cv::cvtColor(mat_input, hsv_im, CV_RGB2HSV);
    		mat_output = cv::Mat::zeros(mat_input.rows, mat_input.cols, CV_8UC1);
    	}
#endif

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	key_is_down[key] = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	key_is_down[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
	vid_grabber.release();
}
