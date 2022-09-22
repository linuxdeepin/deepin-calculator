// Copyright (C) 2004 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008, 2009, 2010, 2013 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_OPCODE_H
#define CORE_OPCODE_H

#include<QString>


class Opcode
{
public:
    enum  Type { Nop, Load, Ref, Function, Add, Sub, Neg, Mul, Div, Pow,
                 Fact, Modulo, IntDiv, Sal, Sar, BAnd, BOr, Conv, Pct, Yroot, Log,
                 BNand, BNor, BXor, Shl, Shr, Rol, Ror, Rcl, Rcr
               };

    Type type;
    unsigned index;

    // TODO: this is only needed for Conv Op. Maybe refactor this to a smarter place?
    // TODO: only keep a pointer to the string
    QString text;

    Opcode() : type(Nop), index(0) {}
    Opcode(Type t) : type(t), index(0) {}
    Opcode(Type t, QString txt) : type(t), index(0), text(txt) {}
    Opcode(Type t, unsigned i): type(t), index(i) {}
};

#endif // CORE_OPCODE_H
