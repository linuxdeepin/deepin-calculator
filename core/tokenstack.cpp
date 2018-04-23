#include "tokenstack.h"

inline static int opPrecedence(Token::Op op)
{
    int prec;
    switch(op) {
    case Token::Exclamation: prec = 800; break;
    case Token::Caret: prec = 700; break;
    case Token::Percent: prec = 800; break;
        // Not really an operator but needed for managing shift/reduce conflicts.
    case Token::Function: prec = 600; break;
    case Token::Asterisk:
    case Token::Slash: prec = 500; break;
    case Token::Modulo:
    case Token::Backslash: prec = 600; break;
    case Token::Plus:
    case Token::Minus: prec = 300; break;
    case Token::LeftShift:
    case Token::RightShift: prec = 200; break;
    case Token::Ampersand: prec = 100; break;
    case Token::Pipe: prec = 50; break;
    case Token::RightArrow: prec = 0;
    case Token::RightPar:
    case Token::Semicolon: prec = -100; break;
    case Token::LeftPar: prec = -200; break;
    default: prec = -200; break;
    }

    return prec;
}

inline static bool tokenPositionCompare(const Token& a, const Token& b)
{
    return (a.pos() < b.pos());
}

TokenStack::TokenStack() : QVector<Token>()
{
    topIndex = 0;
    m_error = "";
    ensureSpace();
}

bool TokenStack::isEmpty() const
{
    return topIndex == 0;
}

unsigned TokenStack::itemCount() const
{
    return topIndex;
}

void TokenStack::push(const Token& token)
{
    ensureSpace();
    (*this)[topIndex++] = token;
}

Token TokenStack::pop()
{
    if (topIndex > 0)
        return Token(at(--topIndex));

    m_error = "token stack is empty (BUG)";
    return Token();
}

const Token& TokenStack::top()
{
    return top(0);
}

const Token& TokenStack::top(unsigned index)
{
    return (topIndex > (int)index) ? at(topIndex - index - 1) : Token::null;
}

void TokenStack::ensureSpace()
{
    int length = size();
    while (topIndex >= length) {
        length += 10;
        resize(length);
    }
}

/** Remove \a count tokens from the top of the stack, add a stxAbstract token to the top
 * and adjust its text position and minimum precedence.
 *
 * \param minPrecedence minimum precedence to set the top token, or \c INVALID_PRECEDENCE
 * if this method should use the minimum value from the removed tokens.
 */

void TokenStack::reduce(int count, int minPrecedence)
{
    // assert(itemCount() > count);

    QList<Token> tokens;
    for (int i = 0 ; i < count ; ++i)
        tokens.append(pop());

    reduce(tokens, Token(Token::stxAbstract), minPrecedence);
}

/** Remove \a count tokens from the top of the stack, push \a top to the top
 * and adjust its text position and minimum precedence.
 *
 * \param minPrecedence minimum precedence to set the top token, or \c INVALID_PRECEDENCE
 * if this method should use the minimum value from the removed tokens.
 */
void TokenStack::reduce(int count, Token&& top, int minPrecedence)
{
    // assert(itemCount() >= count);

    QList<Token> tokens;
    for (int i = 0 ; i < count ; ++i)
        tokens.append(pop());

    reduce(tokens, std::forward<Token>(top), minPrecedence);
}

/** Push \a top to the top and adjust its text position and minimum precedence using \a tokens.
 *
 * \param minPrecedence minimum precedence to set the top token, or \c INVALID_PRECEDENCE
 * if this method should use the minimum value from the removed tokens.
 */
void TokenStack::reduce(QList<Token> tokens, Token&& top, int minPrecedence)
{
    qSort(tokens.begin(), tokens.end(), tokenPositionCompare);

    bool computeMinPrec = (minPrecedence == INVALID_PRECEDENCE);
    int min_prec = computeMinPrec ? MAX_PRECEDENCE : minPrecedence;
    int start = -1, end = -1;
    for (Token& token : tokens) {
        if (computeMinPrec) {
            Token::Op op = token.asOperator();
            if (op != Token::InvalidOp) {
                int prec = opPrecedence(op);
                if (prec < min_prec)
                    min_prec = prec;
            }
        }

        if (token.pos() == -1 && token.size() == -1)
            continue;

        if (token.pos() == -1 || token.size() == -1) {
            continue;
        }

        if (start == -1) {
            start = token.pos();
        } else {

        }

        end = token.pos() + token.size();
    }

    if (start != -1) {
        top.setPos(start);
        top.setSize(end - start);
    }

    top.setMinPrecedence(min_prec);
    push(top);
}
