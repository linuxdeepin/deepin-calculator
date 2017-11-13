#ifndef MATHEX_H
#define MATHEX_H

#include <string>
#include <QString>

using namespace std;

namespace MathExError
{
    const string DIVISOR_ERROR = "除数不能为0";
    const string RADICAND_ERROR = "被开方数不能小于0";
    const string EXTRACT_ROOT_ERROR = "负数不存在平方根";
    const string MOD_ERROR = "模运算两操作数需均为整数";
    const string MODULUS_ERROR = "模数不能为0";
    const string POW_ERROR = "0的幂指数不能为非正数";
    const string FACTORIAL_ERROR = "阶乘数需为自然数";
    const string TAN_ERROR = "正切函数自变量取值不能为kπ+π/2";
    const string ARCSIN_ARCCOS_ERROR = "反正弦和反余弦函数自变量绝对值不能大于1";
    const string ANTILOGARITHM_ERROR = "对数的真数不能为非正数";
    const string UNKNOWN_ERROR = "未知异常";
}

const double PI = 3.14159265358979323846;

class MathEx
{
private:
    double EPS = 1e-4;
public:
    MathEx();
    ~MathEx();

    void set_eps(int e);
    double getRound(double d) const;

    double op_add(const double &op1, const double &op2) const;
    double op_subtract(const double &op1, const double &op2) const;
    double op_multiply(const double &op1, const double &op2) const;
    double op_divide(const double &op1, const double &op2) const;
    double op_mod(const double &op1, const double &op2) const;
    double op_pow(const double &op1, const double &op2) const;
    double op_square_root(const double &op) const;
    double op_extract_root(const double &op1, const double &op2) const;
    double op_factorial(const double &op) const;
    double op_percent(const double &op) const;
    double op_degree_to_radian(const double &op) const;
    double op_sin(const double &op) const;
    double op_cos(const double &op) const;
    double op_tan(const double &op) const;
    double op_arcsin(const double &op) const;
    double op_arccos(const double &op) const;
    double op_arctan(const double &op) const;
    double op_lg(const double &op) const;
    double op_ln(const double &op) const;
};

#endif // !MATHEX_H
