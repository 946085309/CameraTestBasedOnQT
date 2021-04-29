#include "CameraThread.h"

CameraThread::CameraThread(QObject* parent ):
	QThread(parent),
	isExit(false)

{
	
}

void CameraThread::run()
{

	//int i = 0;
	//isExit = false;
	//bool isChaged = false;
	//while (!isExit)
	//{
	//	
	//	Sleep(500);
	//	emit sendStr("Running");
	//}

}

void CameraThread::stopThread()
{
	isExit = true;
	
}
