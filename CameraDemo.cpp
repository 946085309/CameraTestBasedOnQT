#include "CameraDemo.h"
#include"CameraThread.h"
#include"CSampleCaptureHandler.h"
#include<QPixmap>
#include<QMetaType>
#include<opencv.hpp>
#include<QMessageBox>
using namespace cv;
CameraDemo::CameraDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
   
    cameraThread = new CameraThread(this);
    cameraOperator = new CameraOperator();
  
    connectSignalsSlots();
  
}




void CameraDemo::getStr(QString num)
{
    static int  i = 0;
    if (i == 5) {
        this->ui.textEditMsgShow->clear();
        i = 0;
    }
    //this->ui.textEditMsgShow->clear();
    this->ui.textEditMsgShow->append(num);
    i++;
}

CameraDemo::~CameraDemo()
{
    if (cameraThread != nullptr) {
        cameraThread->stopThread();
        delete cameraThread;
        cameraThread = nullptr;
    }
    if (cameraOperator != nullptr) {
        delete cameraOperator;
        cameraOperator = nullptr;
    }
}

void CameraDemo::openCamere()
{
    cameraOperator->openCamera();
    //必须先打开才有
    connect((cameraOperator->pCaptureEventHandler), &CSampleCaptureHandler::sendImage, this, &CameraDemo::getMat);
    connect((cameraOperator->pCaptureEventHandler), &CSampleCaptureHandler::sendProcessed, this, &CameraDemo::getProcessed);
    if (!cameraThread->isRunning()) {
        cameraThread->start();
    }
    
}

void CameraDemo::closeCamera()
{
    cameraOperator->closeCamera();
    if (cameraThread->isRunning()) {
        cameraThread->stopThread();
    }
    
}

void CameraDemo::getProcessed(QImage input)
{
    QPixmap pixmap = QPixmap::fromImage(input);

    int with = ui.labelImageProcess->width();
    int height = ui.labelImageProcess->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    
    ui.labelImageProcess->setPixmap(fitpixmap);
}

void CameraDemo::getMat(QImage input)
{
    int a = 10;
    QPixmap pixmap = QPixmap::fromImage(input);

    int with = ui.labelImage->width();
    int height = ui.labelImage->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

    ui.labelImage->setPixmap(fitpixmap);
   
    
   
}
void CameraDemo::takePic() {
    if (cameraOperator->takePicture())  {
        QMessageBox::information(this, "information", QString::fromLocal8Bit("拍照成功，已保存至image文件夹"));
    }
    else {
        QMessageBox::information(this, "information", QString::fromLocal8Bit("拍照失败，请确保相机成功打开"));
    }
}

void CameraDemo::openCloseWhite() {
    cameraOperator->setBalance();

}
void CameraDemo::connectSignalsSlots()
{
    //连接信号与槽
    connect(cameraThread, &CameraThread::sendStr, this, &CameraDemo::getStr);
    connect(ui.pushButtonOpenCamera, &QPushButton::clicked, this, &CameraDemo::openCamere);
    connect(ui.pushButtonCloseCamera, &QPushButton::clicked, this, &CameraDemo::closeCamera);  
    connect(ui.pushButtonTakePic, &QPushButton::clicked, this, &CameraDemo::takePic);
    connect(ui.pushButtonOpenClose, &QPushButton::clicked, this, &CameraDemo::openCloseWhite);
   
}

//void CameraDemo::closeEvent(QCloseEvent* event)
//{
//  /*  QMessageBox::information(this, "information", QString::fromLocal8Bit("确认关闭")); 
//    cameraOperator->closeCamera();
//    while (cameraThread->isRunning())
//    {
//        cameraThread->stopThread();
//    }*/
//    
//   
//}
