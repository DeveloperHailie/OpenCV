# OpenCV - using C++


## 1. chroma.cpp

- 크로마키 합성 비디오 

- 내 폰 배경을 blue로 설정하고 카메라에 보여주면 폰 화면에 selfie.jpg사진이 합성되어 나타남

- HSV 칼라 영상 변환, thresholding,  morphology를 이용한 잡음 제거, 크로마키 영상 합성 단계로 구성되어 있음

#### * 알고리즘

(1)비디오 화면 캡쳐해서 frmae에 저장, HSV로 imgHSV로 저장

(2)BLUE에 해당하는 부분만 1, 나머지는 0으로 흑백 사진 만듦

(3) (2)사진을 morpology 연산(잡음 제거)(=imgThresholded)

(4) (3)사진의 흰색 부분(=(1)사진에서 파란 부분)을 vector<pair<int,int>>에 push

(5)vector을 sorting하여 사각형의 왼쪽 위 좌표와 오른쪽 아래 좌표를 구함(적절한 padding값(=i)을 이용하여 동떨어져있는 값(잡음)을 좌표로 설정하지 않도록 함)

(6) (5)에서 구한 좌표로 사각형 만듦(find_rect)

(7) 합성하고자 하는 사진(selfie.jpg)을 (5)사각형과 같은 크기로 만듦 

(8) (1)사진(=frame)의 (6)사각형 위치에 (7)사진(selfie)을 copy

(9) (8)사진(=frame)과 (2)사진(=imgThresholded) show

(+) (4)단계의 결과(vector=imgThresholded사진의 흰색부분=imgHSV의파란부분)가 empty이면 이후 단계(5~8)는 수행하지 않는다. 영상에 파란 영역이 없어도 에러가 나지 않기 위함이다.


## 2. knn.cpp
- fashion.jpg에 대해 KNN 사용하여 의상 분류하기

  (1) 키보드 '1' 입력
  : fashion.jpg로 train하고 test함. test 결과 출력
  
  (2) 키보드 '2' 입력
  : fashion.jpg로 train하고 (dress_wall, pants_wall, sneakers_wall, tshirts_wall).png로 test함. test 결과 출력
  
  (3) 키보드 '3' 입력
  : fashion.jpg를 골격화한 이미지로 train하고 test함. 결과 출력

#### * 알고리즘

(1)fashion.png를 grayscale로 read, size를 (600,600)으로 수정. (가로 30개, 세로 30개의 이미지가 들어가 있으므로 하나의 이미지가 20x20으로 설정하기 위함)
{"T-shirt", "pants", "shirt", "dress", "jacket", "shoes", "blouse", "sneakers", "bag", "boots"}로 구성된 labelString 배열을 만든다. 이는 후에 int값으로 index를 받아와 바로 string으로 출력하기 위함이다.

(2)fashoin.png의 각 이미지(20x20)의 특징을 size (400)인 일차원 배열에 저장(=train_features)

(3)fashion.png의 각 이미지의 라벨을 저장(인덱스 0-89는 라벨0, 90-179는 라벨1, … , 810-899는 라벨9), 라벨의 값은 숫자로 저장되는데 이는 나중에 labelString의 index값으로 들어간다.

(4)(2)에서 만든 train_features와 (3)에서 만든 labels을 이용하여 knn train

(5)테스트를 위해 테스트에 사용할 이미지를 read하고 속성을 저장할 차례인데, train 데이터와 test 데이터가 같으므로 생략하고 바로 다음 단계로 넘어간다.

(6)(2)에서 만든 train_features(900개 저장되어 있음) 1개씩 knn findNearest(k=3)하여 분류한 값이 prideictedLabels에 저장된다.

(7) predictedLabels 값(인덱스 0-9 중 분류된 값)과 train_features의 이미지 index(i/90)를 비교하여 answer인지 체크한다.

(8) 분류결과(labelString[(int)predictedLabels.at<float>(0)])와 정답률((7)에서 구한 answer이용)을 출력한다.

 
## 3. line.cpp
- highway1.mp4에서 차선을 추출하여 blue line으로 표시

#### * 알고리즘

(1)highway1 영상 열고 frame 가져오기. 비어있으면 “empty image” 출력 후 종료

(2)Canny()를 통해 frame edge detection(=dst_line)

(3)원본 영상 위에 ‘교차점 최소 개수 50, 직선 최소 길이 5, 점간 최대 거리 10으로 찾은 직선’  그리기

(4)원본영상(직선 그려져 있는)과 edge detection한 영상 show
