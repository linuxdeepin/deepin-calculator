/*
 * 1. @类名:    scientificModule
 * 2. @作者:    夏菁 ut000489
 * 3. @日期:    2020-07-01
 * 4. @说明:    科学模式界面
 */
#ifndef SCIENTIFICMODEL_H
#define SCIENTIFICMODEL_H

#include <QStackedLayout>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "src/widgets/textbutton.h"
#include "src/widgets/iconbutton.h"
#include "src/widgets/basickeypad.h"
#include "src/widgets/memorykeypad.h"
#include "src/widgets/tricombobox.h"
#include "src/widgets/scientifickeypad.h"
#include "src/widgets/inputedit.h"
#include "scihistorywidget.h"
#include "sciexpressionbar.h"
#include "memorywidget.h"

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
    void showOrHideHistory(bool b);
    void checkLineEmpty();
signals:
    void changedeg(int deg);
    void getWindowChanged(int width, int height, bool hishide);

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
    void handleFEStateChanged(bool isdown);
    void handlePageStateChanged();

protected:
    //void paintEvent(QPaintEvent *);

private:
    SciExpressionBar *m_sciexpressionBar;
    ScientificKeyPad *m_scikeypadwidget;
    bool m_insidewidget;
    bool m_avail = false;
    int m_deg = 1;//1-deg 2-rad 3-grad
    bool m_FEisdown = false;
    bool m_Pageisdown = false;
    bool m_isinsidefun = false;//鼠标点击在函数widget内
    bool m_memCalbtn; //m+,m-,ms
    bool m_memRCbtn;//mr,mc
    SciHistoryWidget *m_scihiswidget;
};

#endif // SCIENTIFICMODEL_H
