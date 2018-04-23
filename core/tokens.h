#ifndef TOKENS_H
#define TOKENS_H

#include "core/token.h"
#include <QVector>

class Tokens : public QVector<Token> {
public:
    Tokens() : QVector<Token>(), m_valid(true) { }

    bool valid() const { return m_valid; }
    void setValid(bool v) { m_valid = v; }

#ifdef EVALUATOR_DEBUG
    void append(const Token&);
#endif  /* EVALUATOR_DEBUG */

protected:
    bool m_valid;
};


#endif
