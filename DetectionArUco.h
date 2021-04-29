#pragma once
#include<iostream>
#include<opencv.hpp>
#include "opencv2/aruco.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<algorithm>
#include <fstream>
class DetectionArUco
{
public:
	DetectionArUco();
	~DetectionArUco();
	void detectResult(std::vector<std::string> inputAdress, std::vector<int>& markerIds, std::vector<std::vector<cv::Point2f>>& markerCorners);
	std::map<int, std::vector<cv::Point2f>> getImagMap(cv::Mat& img, std::map<int, cv::Point2f> &center,double& distance);
	cv::Point2f getMarkerCenter(std::vector<cv::Point2f> maker);
	//void matchResult(cv::Mat input, cv::Mat input2, std::string outputAderss);
private:
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

};

