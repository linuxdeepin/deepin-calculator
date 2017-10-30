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
    static string InfixToPostfix(string infix);
    static double posfixCompute(string s);
    static double getResult(string s);
};

#endif // ALGORITHM_H
