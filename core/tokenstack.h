#ifndef TOKENSTACK_H
#define TOKENSTACK_H

#include "token.h"
#include <QVector>

#define MAX_PRECEDENCE      INT_MAX
#define INVALID_PRECEDENCE  INT_MIN

class TokenStack : public QVector<Token>
{
public:
    TokenStack();

    bool isEmpty() const;
    unsigned itemCount() const;
    Token pop();
    void push(const Token& token);
    const Token& top();
    const Token& top(unsigned index);
    bool hasError() const { return !m_error.isEmpty(); }
    QString error() const { return m_error; }

    void reduce(int count, int minPrecedence = INVALID_PRECEDENCE);
    void reduce(int count, Token&& top, int minPrecedence = INVALID_PRECEDENCE);
    void reduce(QList<Token> tokens, Token&& top, int minPrecedence = INVALID_PRECEDENCE);

private:
    void ensureSpace();
    int topIndex;
    QString m_error;
};

#endif
