#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <map>

using namespace std;

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();
    ~Utils();

    static QString getQssContent(const QString &filePath);
    static std::string infixToPostfix(std::string infix);
    static double posfixCompute(std::string s);
    static double expressionCalculate(std::string s);
};	

#endif
