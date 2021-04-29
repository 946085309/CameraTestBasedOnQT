#include "CameraOperator.h"
#include<opencv.hpp>
#include"CSampleCaptureHandler.h"
#include<QWidget>
using namespace std;
CameraOperator::CameraOperator()
{
	//�����¼��ص�����ָ��

	IDeviceOfflineEventHandler* pDeviceOfflineEventHandler = NULL;///<�����¼��ص�����

	IFeatureEventHandler* pFeatureEventHandler = NULL;///<Զ���豸�¼��ص�����

	ICaptureEventHandler* pCaptureEventHandler = NULL;///<�ɼ��ص�����
												  //��ʼ��
	
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
			//ö���豸
			gxdeviceinfo_vector vectorDeviceInfo;
			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
			if (0 == vectorDeviceInfo.size())
			{
				cout << "�޿����豸!" << endl;
				break;
			}
			cout << vectorDeviceInfo[0].GetVendorName() << endl;
			cout << vectorDeviceInfo[0].GetSN() << endl;
			//�򿪵�һ̨�豸�Լ��豸�����һ����
			ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
				vectorDeviceInfo[0].GetSN(),
				GX_ACCESS_EXCLUSIVE);
			ObjStreamPtr = ObjDevicePtr->OpenStream(0);

			//ע���豸�����¼���Ŀǰֻ��ǧ����ϵ�����֧�ִ��¼�֪ͨ��
			hDeviceOffline = NULL;
			pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
			hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);

			//��ȡԶ���豸���Կ�����
			ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

			//�����ع�ʱ��(ʾ����д��us,ֻ��ʾ��,�������������ɹ�������)
			//ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(50);

			////ע��Զ���豸�¼�:�ع�����¼���Ŀǰֻ��ǧ����ϵ�����֧���ع�����¼���
			////ѡ���¼�Դ
			//ObjFeatureControlPtr->GetEnumFeature("EventSelector")->SetValue("ExposureEnd");

			////ʹ���¼�
			//ObjFeatureControlPtr->GetEnumFeature("EventNotification")->SetValue("On");
			//GX_FEATURE_CALLBACK_HANDLE hFeatureEvent = NULL;
			//pFeatureEventHandler = new CSampleFeatureEventHandler();
			//hFeatureEvent = ObjFeatureControlPtr->RegisterFeatureCallback(
			//	"EventExposureEnd",
			//	pFeatureEventHandler,
			//	NULL);

			//ע��ص��ɼ�
			pCaptureEventHandler = new CSampleCaptureHandler();
			ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

			//���Ϳ�������
			ObjStreamPtr->StartGrab();
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
			

			isFinished = true;

		} while (0);

	}

	catch (CGalaxyException& e)

	{
		isFinished = false;
		cout << "������: " << e.GetErrorCode() << endl;

		cout << "����������Ϣ: " << e.what() << endl;

	}

	catch (std::exception& e)

	{
		isFinished = false;
		cout << "����������Ϣ: " << e.what() << endl;

	}



	
}

void CameraOperator::closeCamera()
{
	if (isFinished ==false) {
		return;
	}
	//����ͣ������
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();

	//ע���ɼ��ص�
	ObjStreamPtr->UnregisterCaptureCallback();

	////ע��Զ���豸�¼�
	//ObjFeatureControlPtr->UnregisterFeatureCallback(hFeatureEvent);

	////ע���豸�����¼�
	ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);

	//�ͷ���Դ
	ObjStreamPtr->Close();
	ObjDevicePtr->Close();
	//����ʼ����

	IGXFactory::GetInstance().Uninit();

	isFinished == false;

	//�����¼��ص�ָ��

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
