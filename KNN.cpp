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
	// Test Data : Train Data�� ����

	// train �̹������� Ư¡ ����

	// 900��. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// �� ���� ������ row vector�� ���� train_features�� �����Ѵ�. 
	for (int r = 0; r < 30; r++) {     // ���� ���η� 30��
		for (int c = 0; c < 30; c++) { //���� ���η� 30��
			int i = 0;
			for (int y = 0; y < 20; y++) { // 20x20
				for (int x = 0; x < 20; x++) {
					train_features.at<float>(r * 30 + c, i++)
						= img.at<uchar>(r * 20 + y, c * 20 + x);
				}
			}

		}
	}
	// �� ���� ���� ���� ���̺��� �����Ѵ�. 
	for (int i = 0; i < 900; i++) {  // ���� 0~9�� 90����
		labels.at<float>(i, 0) = (i / 90);
	}

	// �н� �ܰ�
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	ofstream outFile("TestData.txt");
	
	// �׽�Ʈ �ܰ�
	Mat predictedLabels;
	int answerCount = 0;
	for (int i = 0; i < 900; i++) { // ���� 900���� ���ؼ�
		Mat test = train_features.row(i); //train �����͸� test �����ͷ� ���
		knn->findNearest(test, 3, predictedLabels);  // k=3
		float prediction = predictedLabels.at<float>(0);

		if (prediction == labels.at<float>(i, 0)) {
			answerCount++;
		}

		cout << "�׽�Ʈ ����" << i << "�� �� = " << labelString[(int)prediction] << '\n';
		outFile << "�׽�Ʈ ����" << i << "�� �� = " << labelString[(int)prediction] << '\n';
	}

	outFile.close();

	// ����� = ���� ���� / �� ���� x 100
	cout << "����� = " << (float)(answerCount / 900.0) * 100.0 << '\n';
	// k=1 : 100 , k=2 : 89 , k=3 : 87 , k=4 : 85 , k=5 : 84
}

void number2(String ImageName) {
	// fashion.png�� train
	// �ٸ� �̹����� test

	// train �̹������� Ư¡ ����

	// 900��. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// �� ���� ������ row vector�� ���� train_features�� �����Ѵ�. 
	for (int r = 0; r < 30; r++) {     // ���� ���η� 30��
		for (int c = 0; c < 30; c++) { //���� ���η� 30��
			int i = 0;
			for (int y = 0; y < 20; y++) { // 20x20
				for (int x = 0; x < 20; x++) {
					train_features.at<float>(r * 30 + c, i++)
						= img.at<uchar>(r * 20 + y, c * 20 + x);
				}
			}

		}
	}
	// �� ���� ���� ���� ���̺��� �����Ѵ�. 
	for (int i = 0; i < 900; i++) {  // ���� 0~9�� 90����
		labels.at<float>(i, 0) = (i / 90);
	}

	// �н� �ܰ�
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	// test ���� �۾�

	// test �̹��� Ư¡ ����
	// Į�� �����̰�, ����� �ܻ��� �ƴ����� �����ϸ�, �Ƿ��� ũ�� �� ��ġ�� ���� �̹����� ���� ������ ��κ��� �����ϴ� �̹���
	// �ε�� �̹��� �״��, �Լ��̿��ؼ� gray�� �ٲٱ�
	Mat testimg = imread(ImageName);
	if (testimg.empty()) {
		cout << ImageName << " �̸��� �̹��� ����" << endl;
		return;
	}
	resize(testimg, testimg, Size(300, 300)); // ���� �̹��� �ʹ� ŭ

	namedWindow(ImageName, WINDOW_AUTOSIZE);
	imshow(ImageName, testimg);

	cvtColor(testimg, testimg, COLOR_BGR2GRAY);
	resize(testimg, testimg, Size(20, 20)); // �Ʒ��̹����� ũ���  ����



	// 1�� 20x20 
	Mat test_features(1, 400, CV_32FC1);

	int i = 0;
	for (int y = 0; y < 20; y++) { // 20x20
		for (int x = 0; x < 20; x++) {
			test_features.at<float>(0, i++) = testimg.at<uchar>(y, x);
		}
	}


	// �׽�Ʈ �ܰ�
	Mat predictedLabels;
	int answerCount = 0;
	Mat test = test_features; //test ������
	knn->findNearest(test, 71, predictedLabels);  // k=71
	float prediction = predictedLabels.at<float>(0);

	cout << "�׽�Ʈ ������ �� = " << labelString[(int)prediction] << '\n';

}

void number3() {
	// ���ȭ�� test Data
	// Test Data : Train Data�� ����	

	// img -> ���ȭ img (skel)
	Mat thresholdImg;
	threshold(img, thresholdImg, 127, 255, cv::THRESH_BINARY);

	Mat skel(thresholdImg.size(), CV_8UC1, Scalar(0)); // skeleton = 0
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	Mat temp, eroded;
	do
	{
		erode(thresholdImg, eroded, element);
		dilate(eroded, temp, element);
		subtract(thresholdImg, temp, temp); // ���� : outline ���� �κ�
		bitwise_or(skel, temp, skel); //OR : ������ skeleto�� ��ħ.
		eroded.copyTo(thresholdImg);
	} while ((countNonZero(thresholdImg) != 0));

	imshow("���ȭ", skel);


	// trainImg���� Ư¡ ����
	// 900��. 20x20
	Mat train_features(900, 400, CV_32FC1);
	Mat labels(900, 1, CV_32FC1);

	// �� ���� ������ row vector�� ���� train_features�� �����Ѵ�. 
	for (int r = 0; r < 30; r++) {     // ���� ���η� 30��
		for (int c = 0; c < 30; c++) { //���� ���η� 30��
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
	// �� ���� ���� ���� ���̺��� �����Ѵ�. 
	for (int i = 0; i < 900; i++) {  // ���� 0~9�� 90����
		int originalLabel = i / 90;

		// top�� ��� label 0
		if (originalLabel == 0 || originalLabel == 2 || originalLabel == 4 || originalLabel == 6) {
			labels.at<float>(i, 0) = 0;
		}
		// pants�� ��� label 1
		else if (originalLabel == 1) {
			labels.at<float>(i, 0) = 1;
		}
		// dress�� ��� label 2
		else if (originalLabel == 3) {
			labels.at<float>(i, 0) = 2;
		}
		// shoes�� ��� label 3
		else if (originalLabel == 5) {
			labels.at<float>(i, 0) = 3;
		}
		// sneakers�� ��� label 4
		else if (originalLabel == 7) {
			labels.at<float>(i, 0) = 4;
		}
		// bag�� ��� label 5
		else if (originalLabel == 8) {
			labels.at<float>(i, 0) = 5;
		}
		// boots�� ��� label 6
		else if (originalLabel == 9) {
			labels.at<float>(i, 0) = 6;
		}
	}

	// �н� �ܰ�
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
	knn->train(trainData);

	ofstream outFile("ThinningData.txt");

	// �׽�Ʈ �ܰ�
	Mat predictedLabels;
	int answerCount = 0;
	for (int i = 0; i < 900; i++) { // ���� 900���� ���ؼ�
		Mat test = train_features.row(i); //train �����͸� test �����ͷ� ���
		knn->findNearest(test, 3, predictedLabels);  // k=3
		float prediction = predictedLabels.at<float>(0);

		if (prediction == labels.at<float>(i, 0)) {
			answerCount++;
		}
		outFile << "Thinning �׽�Ʈ ����" << i << "�� �� = " << skelLabelString[(int)prediction] << '\n';
		cout << "Thinning �׽�Ʈ ����" << i << "�� �� = " << skelLabelString[(int)prediction] << '\n';
	}
	outFile.close();
	// ����� = ���� ���� / �� ���� x 100
	cout << "����� = " << (float)(answerCount / 900.0) * 100.0 << '\n';

}

int main() {

	int key;
	img = imread("fashion.png", IMREAD_GRAYSCALE);
	resize(img, img, Size(600, 600)); // ���ο� 30��, ���ο� 30�� => �ϳ� �� 20x20
	imshow("original", img);

	while (1) {
		cout << "1 or 2 or 3 or q�� �Է��ϼ���" << endl;
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