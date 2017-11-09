#ifndef UTILS_H
#define UTILS_H

#include <QObject>

using namespace std;

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();
    ~Utils();

    static QString getQssContent(const QString &filePath);
    static double compute(const string &expression);
};

#endif
