#include "DetectionArUco.h"
using namespace std;
using namespace cv;


DetectionArUco::DetectionArUco()
{
}


DetectionArUco::~DetectionArUco()
{
}

void DetectionArUco::detectResult(std::vector<std::string> inputAdress, std::vector<int>& markerIds, std::vector<std::vector<cv::Point2f>>& markerCorners)
{

}

std::map<int, std::vector<cv::Point2f>> DetectionArUco::getImagMap(cv::Mat & img, std::map<int, cv::Point2f>& center,double& distance)
{
	std::map<int, std::vector<cv::Point2f>> tempMap;
	std::map<int, Point2f> tempCenter;
	tempMap.clear();
	vector<Point2f> centerPoint;
	this->markerIds.clear();
	this->markerCorners.clear();
	this->rejectedCandidates.clear();
	Mat imgShow;
	//namedWindow("before detection", WINDOW_NORMAL);
	//imshow("before detection", img);
	//waitKey(10);
	img.copyTo(imgShow);
	Mat grayImag;
	cvtColor(img, grayImag, COLOR_BGR2GRAY);
	cv::Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();

	parameters->adaptiveThreshWinSizeMin = 3;
	parameters->adaptiveThreshWinSizeMax = 23;//Ӱ��ѭ������
	parameters->adaptiveThreshConstant = 7;//��ֵ���Ժ��������������������
	parameters->minMarkerPerimeterRate = 0.2;//��Сmarker�ܳ�
	parameters->maxMarkerPerimeterRate = 8;//���marker�ܳ�
	parameters->polygonalApproxAccuracyRate = 0.01;//��ֵ
	parameters->minCornerDistanceRate = 0.15;
	parameters->errorCorrectionRate = 3;
	parameters->maxErroneousBitsInBorderRate = 0.2;

	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);

	cv::aruco::detectMarkers(img, dictionary, this->markerCorners, this->markerIds, parameters, this->rejectedCandidates);//�����

	if (this->markerCorners.empty()) { 
		
		cout << rejectedCandidates.size() << endl;
		cout << "markerCorners is empty" << endl;
		return tempMap; }

	//��������ȡ
	for (int i = 0; i < markerCorners.size(); i++) {
		cornerSubPix(grayImag, markerCorners[i], Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::Type::
			EPS | TermCriteria::Type::MAX_ITER, 20, 0.1));

		cout << "��" << i << "��marker������Ͻ�����Ϊ��(" << markerCorners[i][0].x << "," << markerCorners[i][0].y << ")" << endl;
		auto point = this->getMarkerCenter(markerCorners[i]);
		centerPoint.push_back(point);
		cout << "��" << i << "��marker�����������Ϊ��(" << point.x << "," << point.y << ")" << endl;
		//�����Ļ���ͼƬ��
		cv::circle(img, Point((int)point.x, (int)point.y), 2, Scalar(0, 0, 255));
	}
	cout << "----------------------------------------------" << endl;
	for (int i = 0; i < this->markerIds.size(); i++) {
		pair<int, std::vector<cv::Point2f>> p{ this->markerIds[i],this->markerCorners[i] };
		pair<int, Point2f> p2{ this->markerIds[i] ,centerPoint[i] };
		tempMap.insert(p);
		center.insert(p2);
	}
	distance = sqrt(pow(markerCorners[0][0].x - markerCorners[0][1].x, 2) + pow(markerCorners[0][0].y - markerCorners[0][1].y, 2));
	cv::aruco::drawDetectedMarkers(img, markerCorners, markerIds);

	//namedWindow("after detection", WINDOW_NORMAL);
	//imshow("after detection", img);
	//imwrite(input2, imgShow);
	//waitKey(10);
	return tempMap;
}

cv::Point2f DetectionArUco::getMarkerCenter(std::vector<cv::Point2f> maker)
{
	float totalX = 0;
	float totalY = 0;
	for (int i = 0; i < maker.size(); i++) {
		totalX += maker[i].x / maker.size();
		totalY += maker[i].y / maker.size();
	}
	return Point2f(totalX, totalY);
}

//void DetectionArUco::matchResult(cv::Mat input, cv::Mat input2, std::string outputAderss)
//{
//	if (input.empty() || input2.empty())return;
//	ofstream ofs(outputAderss, ios::app | ios::out);
//	//��ȡ����map��ƥ��
//	std::map<int, Point2f> center[2];
//	auto map1 = this->getImagMap(input, center[0]);
//	auto map2 = this->getImagMap(input2, center[1]);
//	for (auto temp : map1) {
//		auto corner1 = temp.second;
//		auto corner2 = map2[temp.first];
//		Point2f center1{ 0,0 }, center2{ 0,0 };
//		if (corner1.empty() || corner2.empty()) {
//			continue;
//		}
//		for (int i = 0; i < corner1.size(); i++) {
//			double errox = std::abs(corner1[i].x - corner2[i].x);
//			double erroy = std::abs(corner1[i].y - corner2[i].y);
//			center1.x += corner1[i].x / corner1.size();
//			center1.y += corner1[i].y / corner1.size();
//			center2.x += corner2[i].x / corner1.size();
//			center2.y += corner2[i].y / corner1.size();
//			ofs << "ID =" << temp.first << "��" << i << "�������Ϊ:(" << errox << "," << erroy << ")." << endl;
//			cout << "ID =" << temp.first << "��" << i << "�������Ϊ:(" << errox << "," << erroy << ")." << endl;
//		}
//		double errox = std::abs(center1.x - center2.x);
//		double erroy = std::abs(center1.y - center2.y);
//
//		cout << "ID =" << temp.first << "���ĵ����Ϊ:(" << errox << "," << erroy << ")." << endl;
//		ofs << "ID =" << temp.first << "���ĵ����Ϊ:(" << errox << "," << erroy << ")." << endl;
//	}
//	ofs.close();
//
//}
