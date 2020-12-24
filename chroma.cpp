#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

bool compare_row(pair<int, int> a, pair<int, int> b) {
	return a.first < b.first;
}
bool compare_col(pair<int, int> a, pair<int, int> b) {
	return a.second < b.second;
}


int main() {

	VideoCapture cap(0);
	if (!cap.isOpened()) { cout << "file not found" << endl; }

	while (1)
	{
		// 640x480
		Mat imgHSV;
		Mat frame;
		Mat selfie;

		cap >> frame;

		selfie = imread("selfie.jpg"); //�� ���� 
		if (selfie.empty()) {
			cout << "image empty" << endl;
			return -1;
		}

		cvtColor(frame, imgHSV, COLOR_BGR2HSV);
		Mat imgThresholded, imgThresholded2;
		inRange(imgHSV, Scalar(110, 100, 100), Scalar(130, 255, 255), imgThresholded);

		// imgThresholded morphology ����
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, kernel);

		// imgThresholded�� blue point�� point vector�� push
		vector<pair<int, int> > point; // (r,c), (y,x)
		for (int y = 0; y < imgThresholded.rows; ++y) {
			for (int x = 0; x < imgThresholded.cols; ++x) {
				if (imgThresholded.at<uchar>(y, x) == 255) {
					point.push_back(pair<int, int>(y, x));
				}
			}

		}

		// blue ���� ��쿣 �ٷ� next while����
		if (!point.empty()) {
			// top : point�� (small row)
			// bottom : point�� (big row)
			// left : point�� (small col)
			// right : point�� (bit col)
			int t, b, l, r;
			// i = video noise => padding
			int i = 20;
			sort(point.begin(), point.end(), compare_row);
			t = point[i].first;
			b = point[point.size() - 1 - i].first;
			sort(point.begin(), point.end(), compare_col);
			l = point[i].second;
			r = point[point.size() - 1 - i].second;

			Rect find_rect = Rect(l, t, r - l, b - t);
			rectangle(frame, find_rect, Scalar(0, 0, 255), 5);

			if (find_rect.width > 0 && find_rect.height > 0 && !find_rect.empty()) {
				resize(selfie, selfie, Point(find_rect.width, find_rect.height)); //selfie�� blue domain���� resize
				Mat roi(frame, Rect(Point(find_rect.x, find_rect.y), Point(find_rect.x + selfie.cols, find_rect.y + selfie.rows))); //���� ����

				selfie.copyTo(roi);
			}
		}

		imshow("selfie", frame);
		imshow("Thresholded", imgThresholded);

		if (waitKey(30) >= 0) break;
	}

	return 0;
}
