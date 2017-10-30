#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QPainter>

class ListItem : public QObject
{
    Q_OBJECT

public:
    ListItem();
    ~ListItem();

    void drawBackground(const QRect &rect, QPainter *painter);
    void drawContent(const QRect &rect, QPainter *painter, bool isLast);
    void insert(const QString &str);
    void backspace();
    QString getExpression() const;
    QString getResult() const;

private:
    QString expression;
    QString result;
};

#endif
