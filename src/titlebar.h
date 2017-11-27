#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSvgWidget>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

private:
    QHBoxLayout *m_mainLayout;
    QSvgWidget *m_iconWidget;
};

#endif
