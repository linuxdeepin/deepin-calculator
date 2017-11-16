#ifndef METACHARACTER
#define METACHARACTER

#include <string>
#include <map>

struct Metacharacter {
    int type;
    int out_priority;
    int in_priority;
    int operand;
    int position;
    string e;
};

const map<string, Metacharacter> METACHARACTERS
{
    { "0",		Metacharacter{ 0,0,0,0,0, "0" }},
    { "1",		Metacharacter{ 0,0,0,0,0, "1" }},
    { "2",		Metacharacter{ 0,0,0,0,0, "2" }},
    { "3",		Metacharacter{ 0,0,0,0,0, "3" }},
    { "4",		Metacharacter{ 0,0,0,0,0, "4" }},
    { "5",		Metacharacter{ 0,0,0,0,0, "5" }},
    { "6",		Metacharacter{ 0,0,0,0,0, "6" }},
    { "7",		Metacharacter{ 0,0,0,0,0, "7" }},
    { "8",		Metacharacter{ 0,0,0,0,0, "8" }},
    { "9",		Metacharacter{ 0,0,0,0,0, "9" }},
    { ".",		Metacharacter{ 0,0,0,0,0, "." }},

    { "+",		Metacharacter{ 1,1,1,2,2, "+" }},
    { "-",		Metacharacter{ 1,1,1,2,2, "-" }},
    { "*",		Metacharacter{ 1,2,2,2,2, "*" }},
    { "/",		Metacharacter{ 1,2,2,2,2, "/" }},

    { "%",		Metacharacter{ 1,2,2,2,2, "%" }},   //求余或百分号
    { "^",		Metacharacter{ 1,3,3,2,2, "^" }},   //乘方
    { "#",		Metacharacter{ 1,3,3,2,2, "#" }},   //开多次方
    { "<",		Metacharacter{ 1,4,4,1,1, "<" }},   //开平方
    { "!",		Metacharacter{ 1,4,4,1,3, "!" }},   //阶层

    { "sin",	Metacharacter{ 1,11,11,1,1, "sin" }},   //正弦（默认弧度）
    { "cos",	Metacharacter{ 1,11,11,1,1, "cos" }},   //余弦
    { "tan",	Metacharacter{ 1,11,11,1,1, "tan" }},   //正切
    { "arcsin",	Metacharacter{ 1,11,11,1,1, "arcsin" }},    //反正弦
    { "arccos",	Metacharacter{ 1,11,11,1,1, "arccos" }},    //反余弦
    { "arctan",	Metacharacter{ 1,11,11,1,1, "arctan" }},    //反正切
    { "lg",		Metacharacter{ 1,11,11,1,1, "lg" }},    //常用对数，以10为底
    { "log",	Metacharacter{ 1,11,11,1,1, "log" }},   //常用对数，以10为底
    { "ln",		Metacharacter{ 1,11,11,1,1, "ln" }},    //自然对数，以e为底

    { "`",		Metacharacter{ 1,20,20,1,3, "`" }},   //度(角度)
    { ">",		Metacharacter{ 1,20,20,1,3, ">" }},   //百分号

    { "(",		Metacharacter{ 2,103,-1,0,0, "(" }},
    { "[",		Metacharacter{ 2,102,-1,0,0, "[" }},
    { "{",		Metacharacter{ 2,101,-1,0,0, "{" }},
    { ")",		Metacharacter{ 2,-1,103,0,0, ")" }},
    { "]",		Metacharacter{ 2,-1,102,0,0, "]" }},
    { "}",		Metacharacter{ 2,-1,101,0,0, "}" }},

    { "$",		Metacharacter{ 3,-10,-10,0,0, "$" }},   //结束符号
};

#endif // METACHARACTER

