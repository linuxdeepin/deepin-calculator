// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Complex number parser. Recognizes forms: a, a+ib, a+jb, a+bi, a+bj,
// ib, jb, bi, bj (where i/j is the imaginary unit).

#include "cnumberparser.h"

using namespace CNumberParserExceptions;

CNumberParser::CNumberParser(const char *_str)
    : str(_str)
{
}

void CNumberParser::parse(CNumber *number)
{
    CNumber a = part();
    CNumber b = suite();
    *number = a + b;
}

CNumber CNumberParser::part()
{
    if (str == nullptr || *str == '\0') {
        return CMath::nan();
    }
    if (strncmp(str, "NaN", 3) == 0) {
        return CMath::nan();
    }
    if (*str == 'i' || *str == 'j') {
        return part_prefixed();
    }
    if (isdigit(*str) || *str == '-' || *str == '+' || *str == '.') {
        return part_postfixed();
    }
    return CMath::nan();
}

CNumber CNumberParser::suite()
{
    if (str == nullptr || *str == '\0') {
        return CNumber(0);
    }
    if (*str == '+') {
        accept();
        return part() + suite();
    }
    if (*str == '-') {
        accept();
        return (-part()) + suite();
    }
    return CNumber(0);
}

CNumber CNumberParser::part_prefixed()
{
    if (*str == 'i' || *str == 'j') {
        accept();
        HNumber y = base_number();
        return CNumber(HNumber(0), y);
    }
    throw UnexpectedSymbol(*str);
}

HNumber CNumberParser::base_number()
{
    return HMath::parse_str(str, &str);
}

CNumber CNumberParser::part_postfixed()
{
    HNumber x = base_number();
    postfix_t p = postfix();
    if (p == REAL) {
        return CNumber(x, 0);
    }
    return CNumber(0, x);
}

CNumberParser::postfix_t CNumberParser::postfix()
{
    if (*str == 'i' || *str == 'j') {
        accept();
        return IMAG;
    }
    return REAL;
}
