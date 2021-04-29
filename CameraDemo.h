#pragma once

#include <QtWidgets/QMainWindow>
#include"CameraThread.h"
#include"CameraOperator.h"
#include "ui_CameraDemo.h"
#include"CSampleCaptureHandler.h"
class CameraDemo : public QMainWindow
{
    Q_OBJECT

public:
    CameraDemo(QWidget *parent = Q_NULLPTR);
    ~CameraDemo();

public slots:
    void getProcessed(QImage);
    void getMat(QImage);
    void getStr(QString);
    void openCamere();
    void closeCamera();
    void takePic();
    void openCloseWhite();
   

private:
    void connectSignalsSlots();
    

private:
   
    Ui::CameraDemoClass ui;
    CameraThread* cameraThread;
    CameraOperator* cameraOperator;
    
private :
   // void closeEvent(QCloseEvent* event);
};
