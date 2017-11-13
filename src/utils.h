#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();
    ~Utils();

    static QString getQssContent(const QString &filePath);
};

#endif
