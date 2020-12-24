#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
#include <fstream>
using namespace cv;
using namespace std;

String labelString[10] = { "T-shirt", "pants", "shirt", "dress", "jacket", "shoes", "blouse", "sneakers", "bag", "boots" };
String skelLabelString[7] = { "Top", "pants", "dress", "shoes", "sneakers", "bag", "boots" };

Mat img;

void number1() {
	// Test Data : Train Data와 동일

	// train 이미지에서 특징 추출

	// 900개. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다. 
	for (int r = 0; r < 30; r++) {     // 숫자 세로로 30개
		for (int c = 0; c < 30; c++) { //숫자 가로로 30개
			int i = 0;
			for (int y = 0; y < 20; y++) { // 20x20
				for (int x = 0; x < 20; x++) {
					train_features.at<float>(r * 30 + c, i++)
						= img.at<uchar>(r * 20 + y, c * 20 + x);
				}
			}

		}
	}
	// 각 숫자 영상에 대한 레이블을 저장한다. 
	for (int i = 0; i < 900; i++) {  // 숫자 0~9가 90개씩
		labels.at<float>(i, 0) = (i / 90);
	}

	// 학습 단계
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	ofstream outFile("TestData.txt");
	
	// 테스트 단계
	Mat predictedLabels;
	int answerCount = 0;
	for (int i = 0; i < 900; i++) { // 숫자 900개에 대해서
		Mat test = train_features.row(i); //train 데이터를 test 데이터로 사용
		knn->findNearest(test, 3, predictedLabels);  // k=3
		float prediction = predictedLabels.at<float>(0);

		if (prediction == labels.at<float>(i, 0)) {
			answerCount++;
		}

		cout << "테스트 샘플" << i << "의 라벨 = " << labelString[(int)prediction] << '\n';
		outFile << "테스트 샘플" << i << "의 라벨 = " << labelString[(int)prediction] << '\n';
	}

	outFile.close();

	// 정답률 = 맞힌 개수 / 총 개수 x 100
	cout << "정답률 = " << (float)(answerCount / 900.0) * 100.0 << '\n';
	// k=1 : 100 , k=2 : 89 , k=3 : 87 , k=4 : 85 , k=5 : 84
}

void number2(String ImageName) {
	// fashion.png로 train
	// 다른 이미지로 test

	// train 이미지에서 특징 추출

	// 900개. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다. 
	for (int r = 0; r < 30; r++) {     // 숫자 세로로 30개
		for (int c = 0; c < 30; c++) { //숫자 가로로 30개
			int i = 0;
			for (int y = 0; y < 20; y++) { // 20x20
				for (int x = 0; x < 20; x++) {
					train_features.at<float>(r * 30 + c, i++)
						= img.at<uchar>(r * 20 + y, c * 20 + x);
				}
			}

		}
	}
	// 각 숫자 영상에 대한 레이블을 저장한다. 
	for (int i = 0; i < 900; i++) {  // 숫자 0~9가 90개씩
		labels.at<float>(i, 0) = (i / 90);
	}

	// 학습 단계
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	// test 위한 작업

	// test 이미지 특징 추출
	// 칼라 영상이고, 배경은 단색은 아니지만 심플하며, 의류의 크기 및 배치는 샘플 이미지와 같이 영상의 대부분을 차지하는 이미지
	// 로드는 이미지 그대로, 함수이용해서 gray로 바꾸기
	Mat testimg = imread(ImageName);
	if (testimg.empty()) {
		cout << ImageName << " 이름의 이미지 없음" << endl;
		return;
	}
	resize(testimg, testimg, Size(300, 300)); // 기존 이미지 너무 큼

	namedWindow(ImageName, WINDOW_AUTOSIZE);
	imshow(ImageName, testimg);

	cvtColor(testimg, testimg, COLOR_BGR2GRAY);
	resize(testimg, testimg, Size(20, 20)); // 훈련이미지의 크기와  같게



	// 1개 20x20 
	Mat test_features(1, 400, CV_32FC1);

	int i = 0;
	for (int y = 0; y < 20; y++) { // 20x20
		for (int x = 0; x < 20; x++) {
			test_features.at<float>(0, i++) = testimg.at<uchar>(y, x);
		}
	}


	// 테스트 단계
	Mat predictedLabels;
	int answerCount = 0;
	Mat test = test_features; //test 데이터
	knn->findNearest(test, 71, predictedLabels);  // k=71
	float prediction = predictedLabels.at<float>(0);

	cout << "테스트 샘플의 라벨 = " << labelString[(int)prediction] << '\n';

}

void number3() {
	// 골격화한 test Data
	// Test Data : Train Data와 동일	

	// img -> 골격화 img (skel)
	Mat thresholdImg;
	threshold(img, thresholdImg, 127, 255, cv::THRESH_BINARY);

	Mat skel(thresholdImg.size(), CV_8UC1, Scalar(0)); // skeleton = 0
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	Mat temp, eroded;
	do
	{
		erode(thresholdImg, eroded, element);
		dilate(eroded, temp, element);
		subtract(thresholdImg, temp, temp); // 빼기 : outline 돌출 부분
		bitwise_or(skel, temp, skel); //OR : 기존의 skeleto에 합침.
		eroded.copyTo(thresholdImg);
	} while ((countNonZero(thresholdImg) != 0));

	imshow("골격화", skel);


	// trainImg에서 특징 추출
	// 900개. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다. 
	for (int r = 0; r < 30; r++) {     // 숫자 세로로 30개
		for (int c = 0; c < 30; c++) { //숫자 가로로 30개
			int i = 0;
			for (int y = 0; y < 20; y++) { // 20x20
				for (int x = 0; x < 20; x++) {
					train_features.at<float>(r * 30 + c, i++)
						= skel.at<uchar>(r * 20 + y, c * 20 + x);
				}
			}

		}
	}

	/*
	T-shirt, pants, shirt, dress, jacket, shoes, blouse, sneakers, bag, boots
	Top,      pants, top, dress,  top,    shoes, top,    sneakers,  bag, boots
	Top, pants, dress, shoes, sneakers, bag, boots
	*/
	// 각 숫자 영상에 대한 레이블을 저장한다. 
	for (int i = 0; i < 900; i++) {  // 숫자 0~9가 90개씩
		int originalLabel = i / 90;

		// top인 경우 label 0
		if (originalLabel == 0 || originalLabel == 2 || originalLabel == 4 || originalLabel == 6) {
			labels.at<float>(i, 0) = 0;
		}
		// pants인 경우 label 1
		else if (originalLabel == 1) {
			labels.at<float>(i, 0) = 1;
		}
		// dress인 경우 label 2
		else if (originalLabel == 3) {
			labels.at<float>(i, 0) = 2;
		}
		// shoes인 경우 label 3
		else if (originalLabel == 5) {
			labels.at<float>(i, 0) = 3;
		}
		// sneakers인 경우 label 4
		else if (originalLabel == 7) {
			labels.at<float>(i, 0) = 4;
		}
		// bag인 경우 label 5
		else if (originalLabel == 8) {
			labels.at<float>(i, 0) = 5;
		}
		// boots인 경우 label 6
		else if (originalLabel == 9) {
			labels.at<float>(i, 0) = 6;
		}
	}

	// 학습 단계
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	ofstream outFile("ThinningData.txt");

	// 테스트 단계
	Mat predictedLabels;
	int answerCount = 0;
	for (int i = 0; i < 900; i++) { // 숫자 900개에 대해서
		Mat test = train_features.row(i); //train 데이터를 test 데이터로 사용
		knn->findNearest(test, 3, predictedLabels);  // k=3
		float prediction = predictedLabels.at<float>(0);

		if (prediction == labels.at<float>(i, 0)) {
			answerCount++;
		}
		outFile << "Thinning 테스트 샘플" << i << "의 라벨 = " << skelLabelString[(int)prediction] << '\n';
		cout << "Thinning 테스트 샘플" << i << "의 라벨 = " << skelLabelString[(int)prediction] << '\n';
	}
	outFile.close();
	// 정답률 = 맞힌 개수 / 총 개수 x 100
	cout << "정답률 = " << (float)(answerCount / 900.0) * 100.0 << '\n';

}

int main() {

	int key;
	img = imread("fashion.png", IMREAD_GRAYSCALE);
	resize(img, img, Size(600, 600)); // 가로에 30개, 세로에 30개 => 하나 당 20x20
	imshow("original", img);

	while (1) {
		cout << "1 or 2 or 3 or q를 입력하세요" << endl;
		key = waitKeyEx();

		if (key == '1') {
			number1();
		}
		else if (key == '2') {
			number2("dress_wall.png");
			number2("pants_wall.png");
			number2("sneakers_wall.png");
			number2("tshirts_wall.png");
		}
		else if (key == '3') {
			number3();
		}
		else if (key == 'q') {
			break;
		}
	}

	return 0;
}