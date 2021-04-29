#pragma once
#include<QThread.h>
#include"CSampleCaptureHandler.h"
class CameraThread :
    public QThread
{
    Q_OBJECT
public:
    CameraThread(QObject* parent = nullptr);
    virtual void run();
    void stopThread();

signals:
    void sendStr(QString);

private:
    bool isExit;
    CSampleCaptureHandler* csample;
};

