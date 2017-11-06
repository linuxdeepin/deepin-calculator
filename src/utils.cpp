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

string Utils::InfixToPostfix(string infix)
{
    char current = 0;
    string postfix;

    stack<char> mark;

    map<char,int> priority;
    priority['+'] = 0;
    priority['-'] = 0;
    priority['*'] = 1;
    priority['/'] = 1;
    priority['%'] = 1;

    for(int i = 0; i < infix.size(); ++i)
    {
        current = infix[i];
        switch(current)
        {
        case '0':case '1':case '2':case '3':case '4':case '5':
        case '6':case '7':case '8':case '9':case '.':
            postfix.push_back(current);
            break;

        case '+':case '-':case '*':case '/':case '%':
            if(infix[i-1] != ')')
                postfix.push_back('#');

            if(!mark.empty())
            {
                char tempTop = mark.top();
                while(tempTop != '(' && priority[current] <= priority[tempTop])
                {
                    postfix.push_back(tempTop);
                    mark.pop();
                    if(mark.empty())
                        break;
                    tempTop = mark.top();
                }
                }
                mark.push(current);
                break;

        case '(':
            if(infix[i-1] >= '0' && infix[i-1] <= '9')
            {
                postfix.push_back('#');
                mark.push('*');
            }
            mark.push(current);
            break;

        case ')':
            postfix.push_back('#');
            while(mark.top() != '(')
            {
                postfix.push_back(mark.top());
                mark.pop();
            }
            mark.pop();
                break;

        default:
            break;
        }
    }
    if(infix[infix.size()-1] != ')')
        postfix.push_back('#');
    while(!mark.empty())
    {
        postfix.push_back(mark.top());
        mark.pop();
    }
    return postfix;
}


double Utils::posfixCompute(string s)
{
    stack<double> tempResult;

    string strNum;
    double currNum = 0;
    double tempNum = 0;

    for (string::const_iterator i = s.begin(); i != s.end(); ++i)
    {
        switch(*i)
        {
        case '0':case '1':case '2':case '3':case '4':case '5':
        case '6':case '7':case '8':case '9':case '.':
            strNum.push_back(*i);
            break;

        case '%':
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum = (int)tempResult.top() % (int)tempNum;
            tempResult.pop();
            tempResult.push(tempNum);
            break;

        case '+':
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum += tempResult.top();
            tempResult.pop();
            tempResult.push(tempNum);
            break;

        case '-':
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum = tempResult.top() - tempNum;
            tempResult.pop();
            tempResult.push(tempNum);
            break;

        case '*':
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum *= tempResult.top();
            tempResult.pop();
            tempResult.push(tempNum);
            break;

        case '/':
            tempNum = tempResult.top();
            tempResult.pop();
            tempNum = tempResult.top() / tempNum;
            tempResult.pop();
            tempResult.push(tempNum);
            break;

        case '#':
            currNum = atof(strNum.c_str());
            strNum.clear();
            tempResult.push(currNum);
            break;
        }
    }

    return tempResult.top();
}

double Utils::getResult(string s)
{
    return posfixCompute(InfixToPostfix(s));
}
