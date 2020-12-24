#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

void draw_line(Mat& src_line, Mat dst_line, int thr_line_1, int thr_line_2, int thr_line_3);

int main(){
	string videoName = "highway1.mp4";
	VideoCapture cap(videoName);

	if (!cap.isOpened()) {
		cout << "file not found!" << endl; 
	}

	while (1) {
		Mat src_line, dst_line;
		cap >> src_line;
		if (src_line.empty()) {
			cout << "empty imgae" << endl;
			break;
		}
		Canny(src_line, dst_line, 100, 200); // edge detection
		draw_line(src_line, dst_line, 50, 5, 10); 
		imshow("원본 영상", src_line);
		imshow("edge 영상", dst_line);
		waitKey(30);
	}

	return 0;
}

void draw_line(Mat& src_line, Mat dst_line, int thr_line_1, int thr_line_2, int thr_line_3) {
	vector<Vec4i> lines(0);
	HoughLinesP(dst_line, lines, 1, CV_PI / 180, thr_line_1, thr_line_2, thr_line_3); 
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(src_line, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 3, LINE_AA);
	}
}
