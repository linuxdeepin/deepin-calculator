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

    void drawBackground(QRect rect, QPainter *painter);
    void drawContent(QRect rect, QPainter *painter);

private:
    QString express;
};

#endif
