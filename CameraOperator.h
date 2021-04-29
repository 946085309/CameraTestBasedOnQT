#pragma once
#include"GalaxyIncludes.h"
#include"CSampleCaptureHandler.h"
#include<iostream>
#include<QObject>

enum class CamereParameters {
	BALANCE_WHITE_NONE = 0,
	BALANCE_WHITE_AUTO = 1,
	SHUTTER_VALUE_50000 = 50000,
	SHUTTER_VALUE_40000 = 40000,
	SHUTTER_VALUE_30000 = 30000,
	SHUTTER_VALUE_20000 = 20000,
	SHUTTER_VALUE_10000 = 10000
};

//----------------------------------------------------------------------------------
/**
\brief     �ж��Ƿ����
\param     pBmpInfo BITMAPINFOָ��
\param     nWidth ͼ���
\param     nHeight ͼ���
\return    trueΪһ����false��һ��
*/
//----------------------------------------------------------------------------------
bool __IsCompatible(BITMAPINFO* pBmpInfo, uint64_t nWidth, uint64_t nHeight);

//�û��̳е����¼�������

class CSampleDeviceOfflineEventHandler : public IDeviceOfflineEventHandler

{

public:

	void DoOnDeviceOfflineEvent(void* pUserParam)

	{

		std::cout << "�յ��豸�����¼�!" << std::endl;

	}

};



//�û��̳����Ը����¼�������

class CSampleFeatureEventHandler : public IFeatureEventHandler

{

public:

	void DoOnFeatureEvent(const GxIAPICPP::gxstring& strFeatureName, void* pUserParam)

	{

		std::cout << "�յ��ع�����¼�!" << std::endl;

	}

};



//�û��̳вɼ��¼�������


class CameraOperator
{
	
public:
	CameraOperator();
	~CameraOperator();

	//��
	void openCamera();
	//�ر�
	void closeCamera();
	//������
	void setBalance();
	
	bool takePicture();

	bool isFinished = false;
public:
	CSampleCaptureHandler* pCaptureEventHandler;///<�ɼ��ص�����
private:
	IDeviceOfflineEventHandler* pDeviceOfflineEventHandler;///<�����¼��ص�����

	IFeatureEventHandler* pFeatureEventHandler;///<Զ���豸�¼��ص�����

	
												  //��ʼ��

	CGXDevicePointer ObjDevicePtr;
	CGXStreamPointer ObjStreamPtr;
	CGXFeatureControlPointer ObjFeatureControlPtr;
	CGXFeatureControlPointer objStreamFeatureControlPtr;

	//
	GX_FEATURE_CALLBACK_HANDLE hFeatureEvent;
	GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline;
	bool isOpenWhiteBalance = false;
	
};

