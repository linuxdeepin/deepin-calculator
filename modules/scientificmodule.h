#ifndef SCIENTIFICMODEL_H
#define SCIENTIFICMODEL_H

#include <DWidget>
#include <DGuiApplicationHelper>
#include <QStackedLayout>
#include <QGridLayout>

#include "../widgets/textbutton.h"
#include "../widgets/iconbutton.h"
#include "../widgets/basickeypad.h"
#include "../widgets/memorykeypad.h"
#include "../widgets/tricombobox.h"
#include "expressionbar.h"
#include "memorywidget.h"
#include "./widgets/scibasickeypad.h"
#include "../widgets/inputedit.h"
#include "scihistorywidget.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class scientificModule : public DWidget
{
    Q_OBJECT

public:
    scientificModule(QWidget *parent = nullptr);
    ~scientificModule();
    void setKeyPress(QKeyEvent *e);

    void mousePressEvent(QMouseEvent *event);

public slots:
    //memory func
    void mAvailableEvent();
    void mUnAvailableEvent();
private slots:
    void initTheme(int type);
private:
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void equalButtonPress();
    void handleClearStateChanged(bool isAllClear);
    void handleDegChanged();

protected:
    //void paintEvent(QPaintEvent *);

private:
    ExpressionBar *m_expressionBar;
    MemoryKeypad *m_memoryKeypad;
    SciBasicKeyPad *m_scikeypadwidget;
    QColor m_expBarColor;
    QColor m_expBarSepColor;
    QColor m_btnSepColor;
    bool m_insidewidget;
    bool m_avail = false;
    int m_deg = 1;//1-deg 2-rad 3-grad
    bool m_isinsidefun = false;//鼠标点击在函数widget内
    SciHistoryWidget *m_scihiswidget;
};

#endif // SCIENTIFICMODEL_H
