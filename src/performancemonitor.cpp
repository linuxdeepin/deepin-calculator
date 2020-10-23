#include "performancemonitor.h"

const QString LOG_FLAG = "[PerformanceMonitor]";

const QString GRAB_POINT     = "[GRABPOINT]";
const QString APP_NAME       = "DEEPIN_CALCULATOR";
const QString INIT_APP_TIME  = "0001";
const QString CLOSE_APP_TIME = "0002";
const QString CALCULATE_TIME = "0003";

qint64 PerformanceMonitor::initializeAppStartMs  = 0;
qint64 PerformanceMonitor::inittalizeApoFinishMs = 0;
qint64 PerformanceMonitor::closeAppStartMs       = 0;
qint64 PerformanceMonitor::closeAppFinishMs      = 0;
qint64 PerformanceMonitor::startCalculateMs       = 0;
qint64 PerformanceMonitor::finishCalculateMs      = 0;

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
    qInfo() << QString("%1 %2-%3 %4 #(Init app time)").arg(GRAB_POINT).arg(APP_NAME).arg(INIT_APP_TIME).arg(time);
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
    qInfo() << QString("%1 %2-%3 %4 #(Close app time)").arg(GRAB_POINT).arg(APP_NAME).arg(CLOSE_APP_TIME).arg(time);
}

void PerformanceMonitor::startCalculate()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << "start to calculate";
    startCalculateMs = current.toMSecsSinceEpoch();
}

void PerformanceMonitor::finishCalculate()
{
    QDateTime current = QDateTime::currentDateTime();
    qDebug() << "LOG_FLAG"
             << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
             << " finish to calculate";

    finishCalculateMs = current.toMSecsSinceEpoch();
    qint64 time = finishCalculateMs - startCalculateMs;
    qInfo() << QString("%1 %2-%3 %4 #(Calculate time)").arg(GRAB_POINT).arg(APP_NAME).arg(CALCULATE_TIME).arg(time);
}
