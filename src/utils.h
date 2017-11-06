#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <stack>

using namespace std;

class Utils : public QObject
{
    Q_OBJECT
    
public:
    Utils();
    ~Utils();

    static QString getQssContent(const QString &filePath);
    static string InfixToPostfix(string infix);
    static double posfixCompute(string s);
    static double getResult(string s);
};	

#endif
