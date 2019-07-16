#ifndef BUTTONS_H
#define BUTTONS_H

enum Buttons {
    Key_Clear, Key_Percent, Key_Backspace, Key_Div,
    Key_7, Key_8, Key_9, Key_Mult,
    Key_4, Key_5, Key_6, Key_Min,
    Key_1, Key_2, Key_3, Key_Plus,
    Key_0, Key_Point, Key_Brackets, Key_Equals,
    Key_LBracket, Key_RBracket, Key_sin, Key_cos,
    Key_tan, Key_arcsin, Key_arccos, Key_arctan,
    Key_lg, Key_ln, Key_pi, Key_e, Key_Sqrt,
    Key_Factorial, Key_Power, Key_Square
};

struct KeyDescription {
    QString text;
    Buttons button;
    int row;
    int column;
};

#endif // BUTTONS_H
