#pragma once
#include"GalaxyIncludes.h"
#include<QWidget>
#include<opencv.hpp>
#include<QImage>
#include<QObject>
#include"DetectionArUco.h"
#include<cmath>
class CSampleCaptureHandler : public QObject,public ICaptureEventHandler
{
	Q_OBJECT

public:
	
	virtual void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
	static GX_VALID_BIT_LIST GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);
	static QImage cvMat2QImage(const cv::Mat& mat);
	bool getState();
	void saveImage();

	cv::Mat getImage();
	cv::Mat detectArucoLine(cv::Mat& input);

public:
	bool STOP_BIT = true;

signals:
	void sendImage(QImage);
	void sendProcessed(QImage);
private:
	cv::VideoWriter vw;
	bool isSaved = false;
	cv::Mat output;
	int index = 0;
	
};

