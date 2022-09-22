/* floatlogic.h: logic functions, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATLOGIC_H
# define FLOATLOGIC_H

#include "floatconvert.h"

#ifdef __cplusplus
extern "C" {
#endif

char _canconvert(cfloatnum x);
char _floatnum2logic(t_longint* longint, cfloatnum x);
void _logic2floatnum(floatnum f, t_longint* longint);
void _not(t_longint* longint);
void _and(t_longint* x1, t_longint* x2);
void _or(t_longint* x1, t_longint* x2);
void _xor(t_longint* x1, t_longint* x2);
void _shr(t_longint* x, unsigned shift);
void _shl(t_longint* x, unsigned shift);

#ifdef __cplusplus
}
#endif

#endif /* FLOATLOGIC_H */
