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
    static QString infixToPostfix(const QString &infix);
    static double posfixCompute(const QString &posfix);
    static double expressionCalculate(const QString &expression);
};	

#endif
