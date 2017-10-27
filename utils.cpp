#include "utils.h"
#include <QFile>

Utils::Utils()
{

}

Utils::~Utils()
{
}

QString Utils::getQssContent(const QString &filePath)
{
    QFile file(filePath);
    QString qss = nullptr;
    
    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
    }

    return qss;
}
