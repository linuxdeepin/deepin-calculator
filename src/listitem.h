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

    void drawBackground(const QRect &rect, QPainter *painter, QString color);
    void drawContent(const QRect &rect, QPainter *painter, QString fontColor, QString lastFontColor, bool isLast);
    void initFontSize();

    QString expression;
    QString result;

private:
    int fontSize;
};

#endif
