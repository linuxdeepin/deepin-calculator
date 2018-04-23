#ifndef TOKEN_H
#define TOKEN_H

#include "core/functions.h"
#include "math/hmath.h"
#include "math/cmath.h"
#include "math/quantity.h"

class Token {
public:
    enum Op { InvalidOp = 0, Plus, Minus, Asterisk, Slash, Backslash, Caret, Percent,
              Super0, Super1, Super2, Super3, Super4, Super5, Super6, Super7, Super8, Super9,
              LeftPar, RightPar, Semicolon, Exclamation, Equal, Modulo,
              LeftShift, RightShift, Ampersand, Pipe, RightArrow,
              Function };  /* Not really an operator but needed for managing shift/reduce conflicts */
    enum Type { stxUnknown, stxNumber, stxIdentifier, stxAbstract, stxOperator, stxOpenPar, stxClosePar, stxSep};
    //                     |<-------------isOperand------------->|<----------------isOperator----------------->|

    static const Token null;
    static Token::Op matchOperator(const QString& text);

    Token(Type type = stxUnknown, const QString& text = QString::null, int pos = -1, int size = -1);
    Token(const Token&);

    Quantity asNumber() const;
    Op asOperator() const;
    QString description() const;
    bool isNumber() const { return m_type == stxNumber; }
    bool isOperator() const { return m_type >= stxOperator; }
    bool isIdentifier() const { return m_type == stxIdentifier; }
    bool isOperand() const {return m_type == stxNumber || m_type == stxIdentifier || m_type == stxAbstract;}
    int pos() const { return m_pos; }
    void setPos(int pos) { m_pos = pos; }
    int size() const { return m_size; }
    void setSize(int size) { m_size = size; }
    QString text() const { return m_text; }
    Type type() const { return m_type; }
    int minPrecedence() const { return m_minPrecedence; }
    void setMinPrecedence(int minPrecedence) { m_minPrecedence = minPrecedence; }

    Token& operator=(const Token&);

protected:
    /** Start position of the text that token represents in the expression (might include extra space characters). */
    int m_pos;
    /** Size of text that token represents in the expression (might include extra space characters). */
    int m_size;
    /** Precedence of the operator with the lowest precedence contained in this token. */
    int m_minPrecedence;
    /** Normalized version of that token text (only valid when the token represents a single token). */
    QString m_text;
    Type m_type;
};

#endif
