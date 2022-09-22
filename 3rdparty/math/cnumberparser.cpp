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


#include "cnumberparser.h"

using namespace CNumberParserExceptions;

CNumberParser::CNumberParser(const char * _str) :
    str(_str)
{
}

/* Parses a complex number using a recursive-desent approach. *
 * If successfull, stores the result into *Number.            *
 * See header file for details.                               */
void CNumberParser::parse (CNumber * Number) {
  /* Parsing */
  CNumber a = part();
  CNumber b = suite();
  /* Processing */
  *Number  = a + b;
}

/* Null string or empty string. strlen(NULL) is undefined. */
static char _isempty(const char* p){ return p == NULL || *p == '\0'; }

/* Parsing functions.                                                 *
 * Each of the following function parses one producion of the grammar */
CNumber CNumberParser::part () {
  if (_isempty(str)) {
    return CMath::nan();
  }
  else if (strncmp(str, "NaN", 3) == 0) {
    return CMath::nan();
  }
  else if (*str == 'i' || *str == 'j') {
    return part_prefixed();
  }
  else if (isdigit (*str) || *str == '-' || *str == '+' || *str == '.') {
    /* Example cases : 1.0 -1.0 +1.0 .5 */
    return part_postfixed();
  }
  else {
    return CMath::nan();
  }
}

CNumber CNumberParser::suite () {
  if (_isempty(str)) {
    return CNumber(0);
  }
  else if (*str == '+') {
    accept();
    CNumber a = part();
    CNumber b = suite();
    return a + b;
  }
  else if (*str == '-') {
    accept();
    CNumber a = part();
    CNumber b = suite();
    return (-a) + b;
  }
  else {
    return CNumber(0);
  }
}

CNumber CNumberParser::part_prefixed () {
  if (*str == 'i' || *str == 'j') {
    /* Parsing */
    accept();
    HNumber y = base_number();
    /* Processing */
    HNumber x = HNumber(0);
    CNumber res = CNumber(x, y);
    return res;
  }
  else {
    throw UnexpectedSymbol(*str);
  }
}

HNumber CNumberParser::base_number () {
  /* FIXME ! Error checking ! */
  HNumber x = HMath::parse_str(str, &str);
  return x;
}

CNumber CNumberParser::part_postfixed () {
  /* Parsing */
  HNumber x = base_number();
  postfix_t p = postfix();

  /* Processing */
  if (p == REAL)           /* If real part of a complex number */
    return CNumber(x, 0);
  else if (p == IMAG)      /* If imaginary part                */
    return CNumber(0, x);
  else {                   /* Should never happen              */
    throw LogicError();
  }
}

CNumberParser::postfix_t CNumberParser::postfix () {
  if (*str == 'i' || *str == 'j') {
    accept();
    return IMAG;
  }
  else {
    return REAL;
  }
}

