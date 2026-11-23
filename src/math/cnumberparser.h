// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
#ifndef CNUMBER_PARSER_HXX
#define CNUMBER_PARSER_HXX

#include <ctype.h>
#include <string.h>

#include "hmath.h"
#include "cmath.h"

class CNumberParser {
  private:
    const char * str;  /* Remaining of the string to parse */

    /* Parsing functions. */
    void accept () {str++;}
    /* Each of the following function parses one producion of the grammar. */
    CNumber part ();
    CNumber suite ();
    CNumber part_prefixed ();
    HNumber base_number ();
    CNumber part_postfixed ();
    typedef enum {IMAG, REAL} postfix_t;
    postfix_t postfix ();

  public:
    CNumberParser (const char * _str);
    void parse (CNumber * number);
};

namespace CNumberParserExceptions {

  class Exception
      : public std::exception {};

  class UnexpectedSymbol
      : public Exception {
    private:
      char symbol;
    public:
      char get_symbol () {return symbol;}
      UnexpectedSymbol (char _symbol) : symbol(_symbol) {}
      /* FIXME ! Error message ! */
  };

  class LogicError
      : Exception {};

  class UnexpectedEnd
      : public Exception {};

}

#endif // CNUMBER_PARSER_HXX
