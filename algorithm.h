#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <stack>

using namespace std;

class Algorithm
{
public:
    string InfixToPostfix(string infix);
    double posfixCompute(string s);
    double getResult(string s);
};

#endif // ALGORITHM_H
