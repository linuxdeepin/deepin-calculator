#include "token.h"


const Token Token::null;

// Helper function: return operator of given token text e.g. "*" yields Operator::Asterisk.
Token::Op Token::matchOperator(const QString& text)
{
    Token::Op result = Token::InvalidOp;

    if (text.length() == 1) {
        QChar p = text.at(0);
        switch(p.unicode()) {
        case '%': result = Token::Percent; break;
        case '+': result = Token::Plus; break;
        case '-': result = Token::Minus; break;
        case '*': result = Token::Asterisk; break;
        case '/': result = Token::Slash; break;
        case '^': result = Token::Caret; break;
        case ';': result = Token::Semicolon; break;
        case '(': result = Token::LeftPar; break;
        case ')': result = Token::RightPar; break;
        case '!': result = Token::Exclamation; break;
        case '=': result = Token::Equal; break;
        case '\\': result = Token::Backslash; break;
        case '&': result = Token::Ampersand; break;
        case '|': result = Token::Pipe; break;
        default: result = Token::InvalidOp;
        }
    }

    // else if (text.length() == 2) {
    //     if (text == "**")
    //             result = Token::Caret;
    //     else if(text == "<<")
    //         result = Token::LeftShift;
    //     else if(text == ">>")
    //         result = Token::RightShift;
    //     else if(text == "->")
    //             result = Token::RightArrow;
    //     else if(text == "in")
    //         result = Token::RightArrow;
    // }

    return result;
}


Token::Token(Type type, const QString& text, int pos, int size)
{
    m_type = type;
    m_text = text;
    m_pos = pos;
    m_size = size;
    m_minPrecedence = INT_MAX;
}

Token::Token(const Token& token)
{
    m_type = token.m_type;
    m_text = token.m_text;
    m_pos = token.m_pos;
    m_size = token.m_size;
    m_minPrecedence = token.m_minPrecedence;
}

Token& Token::operator=(const Token& token)
{
    m_type = token.m_type;
    m_text = token.m_text;
    m_pos = token.m_pos;
    m_size = token.m_size;
    m_minPrecedence = token.m_minPrecedence;
    return*this;
}

Quantity Token::asNumber() const
{
    QString text = m_text;
    return isNumber() ? Quantity(CNumber((const char*)text.toLatin1())) : Quantity(0);
}

Token::Op Token::asOperator() const
{
    return isOperator() ? matchOperator(m_text) : InvalidOp;
}

QString Token::description() const
{
    QString desc;

    switch (m_type) {
    case stxNumber: desc = "Number"; break;
    case stxIdentifier: desc = "Identifier"; break;
    case stxOpenPar:
    case stxClosePar:
    case stxSep:
    case stxOperator: desc = "Operator"; break;
    default: desc = "Unknown"; break;
    }

    while (desc.length() < 10)
        desc.prepend(' ');

    QString header;
    header.append(QString::number(m_pos) + "," + QString::number(m_pos + m_size - 1));
    header.append("," + (m_minPrecedence == INT_MAX ? "MAX" : QString::number(m_minPrecedence)));
    header.append("  ");

    while (header.length() < 10)
        header.append(' ');

    desc.prepend(header);
    desc.append(" : ").append(m_text);

    return desc;
}
