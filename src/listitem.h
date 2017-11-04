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
    void initFontSize();

    QString expression;
    QString result;

private:
    int fontSize;
};

#endif
