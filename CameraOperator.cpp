#include "CameraOperator.h"
#include<opencv.hpp>
#include"CSampleCaptureHandler.h"
#include<QWidget>
using namespace std;
CameraOperator::CameraOperator()
{
	//声明事件回调对象指针

	IDeviceOfflineEventHandler* pDeviceOfflineEventHandler = NULL;///<掉线事件回调对象

	IFeatureEventHandler* pFeatureEventHandler = NULL;///<远端设备事件回调对象

	ICaptureEventHandler* pCaptureEventHandler = NULL;///<采集回调对象
												  //初始化
	
}

CameraOperator::~CameraOperator()
{
	

}

void CameraOperator::openCamera()
{
	IGXFactory::GetInstance().Init();
	isFinished = false;
	try
	{
		do
		{
			//枚举设备
			gxdeviceinfo_vector vectorDeviceInfo;
			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
			if (0 == vectorDeviceInfo.size())
			{
				cout << "无可用设备!" << endl;
				break;
			}
			cout << vectorDeviceInfo[0].GetVendorName() << endl;
			cout << vectorDeviceInfo[0].GetSN() << endl;
			//打开第一台设备以及设备下面第一个流
			ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
				vectorDeviceInfo[0].GetSN(),
				GX_ACCESS_EXCLUSIVE);
			ObjStreamPtr = ObjDevicePtr->OpenStream(0);

			//注册设备掉线事件【目前只有千兆网系列相机支持此事件通知】
			hDeviceOffline = NULL;
			pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
			hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);

			//获取远端设备属性控制器
			ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

			//设置曝光时间(示例中写死us,只是示例,并不代表真正可工作参数)
			//ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(50);

			////注册远端设备事件:曝光结束事件【目前只有千兆网系列相机支持曝光结束事件】
			////选择事件源
			//ObjFeatureControlPtr->GetEnumFeature("EventSelector")->SetValue("ExposureEnd");

			////使能事件
			//ObjFeatureControlPtr->GetEnumFeature("EventNotification")->SetValue("On");
			//GX_FEATURE_CALLBACK_HANDLE hFeatureEvent = NULL;
			//pFeatureEventHandler = new CSampleFeatureEventHandler();
			//hFeatureEvent = ObjFeatureControlPtr->RegisterFeatureCallback(
			//	"EventExposureEnd",
			//	pFeatureEventHandler,
			//	NULL);

			//注册回调采集
			pCaptureEventHandler = new CSampleCaptureHandler();
			ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

			//发送开采命令
			ObjStreamPtr->StartGrab();
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
			

			isFinished = true;

		} while (0);

	}

	catch (CGalaxyException& e)

	{
		isFinished = false;
		cout << "错误码: " << e.GetErrorCode() << endl;

		cout << "错误描述信息: " << e.what() << endl;

	}

	catch (std::exception& e)

	{
		isFinished = false;
		cout << "错误描述信息: " << e.what() << endl;

	}



	
}

void CameraOperator::closeCamera()
{
	if (isFinished ==false) {
		return;
	}
	//发送停采命令
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();

	//注销采集回调
	ObjStreamPtr->UnregisterCaptureCallback();

	////注销远端设备事件
	//ObjFeatureControlPtr->UnregisterFeatureCallback(hFeatureEvent);

	////注销设备掉线事件
	ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);

	//释放资源
	ObjStreamPtr->Close();
	ObjDevicePtr->Close();
	//反初始化库

	IGXFactory::GetInstance().Uninit();

	isFinished == false;

	//销毁事件回调指针

	if (NULL != pCaptureEventHandler)

	{

		delete pCaptureEventHandler;

		pCaptureEventHandler = NULL;

	}

	if (NULL != pDeviceOfflineEventHandler)

	{

		delete pDeviceOfflineEventHandler;

		pDeviceOfflineEventHandler = NULL;

	}

	/*if (NULL != pFeatureEventHandler)

	{

		delete pFeatureEventHandler;

		pFeatureEventHandler = NULL;

	}
	isFinished = false;*/
}

void CameraOperator::setBalance()
{
	bool m_strBalanceWhiteAutoMode = false;
	if (isOpenWhiteBalance == true) {
		m_strBalanceWhiteAutoMode = (bool)CamereParameters::BALANCE_WHITE_NONE;
	}
	else {
		m_strBalanceWhiteAutoMode = (bool)CamereParameters::BALANCE_WHITE_AUTO;
	}
	const char* ContinuousModel = "Continuous";
	if (m_strBalanceWhiteAutoMode) {
		ObjFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(ContinuousModel);
	}
	
}

bool CameraOperator::takePicture()
{
	if (this->isFinished == false) {
		return false;
	}
	pCaptureEventHandler->saveImage();
	return true;
}

bool __IsCompatible(BITMAPINFO* pBmpInfo, uint64_t nWidth, uint64_t nHeight)
{
	
	if (pBmpInfo == NULL
		|| pBmpInfo->bmiHeader.biHeight != nHeight
		|| pBmpInfo->bmiHeader.biWidth != nWidth
		)
	{
		return false;
	}
	return true;
	

}
