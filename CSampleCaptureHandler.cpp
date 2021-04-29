#include "CSampleCaptureHandler.h"
#include<opencv.hpp>
#include<QDebug>
#include<QWidget>
#include<vector>
#include<algorithm>
#include<QImage>

using namespace cv;
using namespace std;
void CSampleCaptureHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	if (!vw.isOpened()) {
		//vw = new VideoWriter;
		vw.open("out.avi", //路径
			CV_FOURCC('M', 'P', '4', '2'), //编码格式
			15.0, //帧率
			Size(objImageDataPointer->GetWidth(),
				objImageDataPointer->GetHeight())  //尺寸
		);
	}
	
	cv::Mat m_pImage;

	if (GX_FRAME_STATUS_SUCCESS == objImageDataPointer->GetStatus())

	{
		STOP_BIT = false;
		uint64_t nWidth = objImageDataPointer->GetWidth();
		uint64_t nHeight = objImageDataPointer->GetHeight();
		m_pImage.create(nHeight, nWidth, CV_8UC3);
		//GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
		BYTE* pBuffer = NULL;

		if (objImageDataPointer.IsNull())
		{
			throw std::runtime_error("NULL pointer dereferenced");
		}

		auto emValidBits = CSampleCaptureHandler::GetBestValudBit(objImageDataPointer->GetPixelFormat());

		pBuffer = (BYTE*)objImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);

		memcpy(m_pImage.data, pBuffer, nHeight * nWidth * 3);
		if (!m_pImage.empty()) {
			if (isSaved) {
				std::string adress = "image//"+std::to_string(this->index++) + ".tif";
				cv::imwrite(adress, m_pImage);
				isSaved = false;
			}

			Mat dst1;
			//pyrDown(m_pImage, dst1);
			//pyrDown(dst1, dst1);
			
		
			auto image = CSampleCaptureHandler::cvMat2QImage(m_pImage);
			
			emit sendImage(image);
			
			/*Mat dst2;
			cv::Mat gray;
			cv::cvtColor(m_pImage, gray, cv::COLOR_BGR2GRAY);*/

			//pyrDown(gray, dst2);
			//pyrDown(dst2, dst2);
			Mat arucoDetected= CSampleCaptureHandler::detectArucoLine(m_pImage);
			auto arucoDetected1 = CSampleCaptureHandler::cvMat2QImage(arucoDetected);
			vw.write(arucoDetected);
			emit sendProcessed(arucoDetected1);
		}
		

		//handle(m_pImage);
		/*cv::namedWindow("ddd", CV_WINDOW_NORMAL);
		cv::imshow("ddd", m_pImage);
		cv::waitKey(10);*/
		STOP_BIT = true;
		

	}
	STOP_BIT = true;
	
}

GX_VALID_BIT_LIST CSampleCaptureHandler::GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	switch (emPixelFormatEntry)
	{
	case GX_PIXEL_FORMAT_MONO8:
	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
	{
		emValidBits = GX_BIT_0_7;
		break;
	}
	case GX_PIXEL_FORMAT_MONO10:
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
	{
		emValidBits = GX_BIT_2_9;
		break;
	}
	case GX_PIXEL_FORMAT_MONO12:
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
	{
		emValidBits = GX_BIT_4_11;
		break;
	}
	case GX_PIXEL_FORMAT_MONO14:
	{
		//暂时没有这样的数据格式待升级
		break;
	}
	case GX_PIXEL_FORMAT_MONO16:
	case GX_PIXEL_FORMAT_BAYER_GR16:
	case GX_PIXEL_FORMAT_BAYER_RG16:
	case GX_PIXEL_FORMAT_BAYER_GB16:
	case GX_PIXEL_FORMAT_BAYER_BG16:
	{
		//暂时没有这样的数据格式待升级
		break;
	}
	default:
		break;
	}
	return emValidBits;
}

QImage CSampleCaptureHandler::cvMat2QImage(const cv::Mat& mat)
{


	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}

	
}

bool CSampleCaptureHandler::getState()
{
	return STOP_BIT;
}

void CSampleCaptureHandler::saveImage()
{
	isSaved = true;
}

cv::Mat CSampleCaptureHandler::getImage()
{
	 return output;
}
bool cmp(cv::Point2f a, cv::Point2f b) {
	return a.x > b.x;
}
cv::Mat CSampleCaptureHandler::detectArucoLine(cv::Mat& input)
{
	DetectionArUco getPoint;
	Mat processedPic;
	//input.copyTo(processedPic);
	
	std::map<int, cv::Point2f> rst;
	rst.clear();
	double distance;
	getPoint.getImagMap(input,rst,distance);
	
	std::vector<cv::Point2f> points;
	for (auto k : rst) {
		points.push_back(k.second);
	}
	std::sort(points.begin(), points.end(), cmp);
	
	if (points.size() > 1) {
		for (unsigned int i = 0; i < points.size() - 1; ++i) {
			line(input, points[i], points[i + 1], Scalar(0, 0, 255), 3, 4);
		}
	}


	Point p(700, 1000);
	circle(input, p, 10, Scalar(0, 0, 255), -1);
	if (points.size()>1) {
		for (int i = 0; i < points.size() - 1; i++) {
			double k = (points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x);
			double b = points[i].y - k * points[i].x;
			if (p.x> points[i + 1].x && p.x < points[i].x) {
				int y = k * p.x + b;
				Point crossPoint;
				crossPoint.x = p.x;
				crossPoint.y = y;
				line(input, p, crossPoint, Scalar(0, 0, 255), 3, 4);
				string s=std::to_string(abs(crossPoint.y - p.y)/distance*3)+" cm";
				Point p2 = p;
				p2.y = (crossPoint.y + p.y) / 2;
				cv::putText(input, s, p2, cv::FONT_HERSHEY_TRIPLEX, 2.5, cv::Scalar(0, 0, 0), 2, CV_AA);
			}
	}
	
		
	}

	return input;
}
