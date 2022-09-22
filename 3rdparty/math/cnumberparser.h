// Copyright (C) 2013, 2015-2016 Hadrien Theveneau <theveneau@gmail.com>.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

// This complex number parser currently recognizes the following forms :
//
//   a
//   a+ib
//   a+jb
//   a+bi
//   a+bj
//   ib
//   jb
//   bi
//   bj
//
// The grammar describing one complex number is:
//
//   complex_number -> part suite
//   suite -> []
//   suite -> + part suite
//
//   part -> prefixed_part
//   part -> postfixed_part
//
//   part_prefixed -> i base_number
//   part_prefixed -> j base_number
//
//   part_postfixed -> base_number postfix
//
//   postfix -> i
//   postfix -> j
//   postfix -> []


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
