#include "utils.h"
#include <QFile>
#include <QStack>
#include <QMap>

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

QString Utils::infixToPostfix(const QString &infix)
{
    QChar current = 0;
    QString postfix;
    QStack<QChar> mark;
    QMap<QChar, int> priority;
    priority['+'] = 0;
    priority['-'] = 0;
    priority['*'] = 1;
    priority['/'] = 1;

    for (int i = 0; i < infix.count(); ++i)
    {
        current = infix.at(i);

        if (current == '0' || current == '1' || current == '2' || current == '3' ||
            current == '4' || current == '5' || current == '6' || current == '7' ||
            current == '8' || current == '9' || current == '.') {
            postfix.append(current);

        } else if (current == '+' || current == '-' || current == '*' || current == '/') {
            if (infix.at(i - 1) != ')') {
                postfix.push_back('#');
            }

            if (!mark.empty()) {
                QChar tempTop = mark.top();
                while (tempTop != '(' && priority[current] <= priority[tempTop]) {
                    postfix.append(tempTop);
                    mark.pop();
                    if (mark.empty()) {
                        break;
                    }
                    tempTop = mark.top();
                }
            }

            mark.push(current);
        } else if (current == '(') {
            if (infix.at(i - 1) >= '0' && infix.at(i - 1) <= '9') {
                postfix.append('#');
                mark.push('*');
            }
            mark.push(current);
        } else if (current == ')') {
            postfix.append('#');
            while (mark.top() != '(') {
                postfix.append(mark.top());
                mark.pop();
            }
            mark.pop();
        }
    }

    if (infix.at(infix.count() - 1) != ')') {
        postfix.append('#');
    }

    while (!mark.empty()) {
        postfix.append(mark.top());
        mark.pop();
    }

    return postfix;
}

double Utils::posfixCompute(const QString &posfix)
{
    QStack<double> tempResult;
    QString strNum;
    double currentNum = 0;
    double tempNum = 0;

    for (QString::const_iterator i = posfix.begin(); i != posfix.end(); ++i) {
        if (*i == '0' || *i == '1' || *i == '2' || *i == '3' || *i == '4' || *i == '5' || *i == '6' || *i == '7' || *i == '8' || *i == '9' || *i == '.') {
        strNum.append(*i);
        } else if (*i == '+') {
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum += tempResult.top();
            tempResult.pop();
            tempResult.push(tempNum);
        } else if (*i == '-') {
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum = tempResult.top() - tempNum;
            tempResult.pop();
            tempResult.push(tempNum);
        } else if (*i == '*') {
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum *= tempResult.top();
            tempResult.pop();
            tempResult.push(tempNum);
        } else if (*i == '/') {
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum = tempResult.top() / tempNum;
            tempResult.pop();
            tempResult.push(tempNum);
        } else if (*i == '#') {
            currentNum = strNum.toDouble();
            strNum.clear();
            tempResult.push(currentNum);
        }
    }

    return tempResult.top();
}

double Utils::expressionCalculate(const QString &expression)
{
    return posfixCompute(infixToPostfix(expression));
}
