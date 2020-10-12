#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->cap_size = cv::Size(360, 360);
	this->rect_size = 20;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->cv_rects.push_back(rect);
			this->rect_frames.push_back(frame);
		}
	}

	this->number_of_frames = 0;
	vector<string> file_path_list = {
		"D:\\MP4\\openframeworks 2020-09-03.mp4",
		"D:\\MP4\\openFrameworks 2020-08-26.mp4",
		"D:\\MP4\\openframeworks 2020-09-29.mp4",
		"D:\\MP4\\openFrameworks 2020-09-18.mp4",
	};

	for (auto& file_path : file_path_list) {

		this->cap.open(file_path);
		int frame_count = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
		for (int i = 0; i < frame_count; i++) {

			cv::Mat src, tmp;
			this->cap >> src;
			if (src.empty()) {

				continue;
			}

			cv::resize(src, tmp, this->cap_size);
			cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

			this->frame_list.push_back(tmp);
		}

		this->number_of_frames += frame_count;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.43);
	ofRotateY(ofGetFrameNum() * 0.65);

	for (int k = 0; k < 6; k++) {

		if (k > 0 && k < 4) { ofRotateY(90); }
		if (k == 4) { ofRotateX(90); }
		if (k == 5) { ofRotateX(180); }


		float noise_seed = ofRandom(1000);
		for (int i = 0; i < this->rect_frames.size(); i++) {

			int frame_index = ofMap(ofNoise(noise_seed, this->cv_rects[i].x * 0.001, this->cv_rects[i].y * 0.001, ofGetFrameNum() * 0.005), 0, 1, 0, this->number_of_frames);

			cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rects[i]);
			tmp_box_image.copyTo(this->rect_frames[i]);

			this->rect_images[i]->update();
			this->rect_images[i]->getTexture().bind();
			ofSetColor(255);
			ofFill();
			ofDrawBox(glm::vec3(
				this->cv_rects[i].x + this->cv_rects[i].width * 0.5 - this->cap_size.width * 0.5,
				this->cap_size.height - this->cv_rects[i].y - this->cv_rects[i].height * 0.5 - this->cap_size.height * 0.5,
				180),
				this->rect_size, this->rect_size, 5);
			this->rect_images[i]->unbind();

			ofSetColor(39);
			ofNoFill();
			ofDrawBox(glm::vec3(
				this->cv_rects[i].x + this->cv_rects[i].width * 0.5 - this->cap_size.width * 0.5,
				this->cap_size.height - this->cv_rects[i].y - this->cv_rects[i].height * 0.5 - this->cap_size.height * 0.5,
				180),
				this->rect_size, this->rect_size, 5);

		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}