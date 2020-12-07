#include "performancemonitor.h"

const QString LOG_FLAG = "[PerformanceMonitor]";

const QString GRAB_POINT     = "[GRABPOINT]";
const QString APP_NAME       = "DEEPIN_CALCULATOR";
const QString INIT_APP_TIME  = "POINT-01";
const QString CLOSE_APP_TIME = "POINT-02";
const QString OPERATE_TIME = "POINT-03";

qint64 PerformanceMonitor::initializeAppStartMs  = 0;
qint64 PerformanceMonitor::inittalizeApoFinishMs = 0;
qint64 PerformanceMonitor::closeAppStartMs       = 0;
qint64 PerformanceMonitor::closeAppFinishMs      = 0;
qint64 PerformanceMonitor::startOperateMs       = 0;
qint64 PerformanceMonitor::finishOperateMs      = 0;

PerformanceMonitor::PerformanceMonitor(QObject *parent) : QObject(parent)
{

}

void PerformanceMonitor::initializeAppStart()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << "start to initaalize app";
    initializeAppStartMs = current.toMSecsSinceEpoch();
}

void PerformanceMonitor::initializAppFinish()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << " finish to initialize app";

    inittalizeApoFinishMs = current.toMSecsSinceEpoch();
    qint64 time = inittalizeApoFinishMs - initializeAppStartMs;
//    qInfo() << QString("%1 %2-%3 %4 #(Init app time)").arg(GRAB_POINT).arg(APP_NAME).arg(INIT_APP_TIME).arg(time);
    qInfo() << QString("%1 %2 time=%3ms").arg(GRAB_POINT).arg(INIT_APP_TIME).arg(time);
}

void PerformanceMonitor::closeAppStart()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << "start to close app";
    closeAppStartMs = current.toMSecsSinceEpoch();
}

void PerformanceMonitor::closeAPPFinish()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << " finish to close app";

    closeAppFinishMs = current.toMSecsSinceEpoch();
    qint64 time = closeAppFinishMs - closeAppStartMs;
//    qInfo() << QString("%1 %2-%3 %4 #(Close app time)").arg(GRAB_POINT).arg(APP_NAME).arg(CLOSE_APP_TIME).arg(time);
    qInfo() << QString("%1 %2 time=%3ms").arg(GRAB_POINT).arg(CLOSE_APP_TIME).arg(time);
}

void PerformanceMonitor::startOperate()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << "start to calculate";
    startOperateMs = current.toMSecsSinceEpoch();
}

void PerformanceMonitor::finishOperate(const QString operate)
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << " finish to calculate";

    finishOperateMs = current.toMSecsSinceEpoch();
    qint64 time = finishOperateMs - startOperateMs;
//    qInfo() << QString("%1 %2-%3 %4 #(Calculate time)").arg(GRAB_POINT).arg(APP_NAME).arg(CALCULATE_TIME).arg(time);
    qInfo() << QString("%1 %2 operationType=%3 time=%4ms").arg(GRAB_POINT).arg(OPERATE_TIME).arg(operate).arg(time);
}
