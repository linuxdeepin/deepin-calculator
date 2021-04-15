#ifndef BASICMODULEVER_H
#define BASICMODULEVER_H

#include <QStackedWidget>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "tablet/control/textbuttontab.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/control/basickeypadtab.h"
#include "tablet/control/memorykeypadtab.h"
#include "widgets/inputedit.h"
#include "expressionbartab.h"
#include "tablet/views/memorywidgettab.h"
#include "tablet/memorypublictab.h"
#include "scihistorywidgettab.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class BasicModuleVer : public DWidget
{
    Q_OBJECT
public:
    BasicModuleVer(QWidget *parent = nullptr);
    ~BasicModuleVer();
    void setKeyPress(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void checkLineEmpty();

signals:
    void sciMemTab();
    void clearbtnShow(bool show);

public slots:
    //memory func
    void mAvailableEvent();
    void mUnAvailableEvent();
    void showListWidget();
    void closeListWidget();
    void titleBarClean();
    void resetWindowSize(QSize size);
private slots:
    void initTheme(int type);
private:
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void handleKeypadButtonPressByspace(int);
    void handleClearStateChanged(bool isAllClear);
    void setBasicTabOrder();

protected:
    //void paintEvent(QPaintEvent *);

private:
    ExpressionBarTab *m_expressionBar;
    BasicKeypadTab *m_basicKeypad;
    MemoryKeypadTab *m_memoryKeypad;
    QStackedWidget *m_keypadLayout;
    bool m_avail = false; //内存中有数据
    bool m_isinsidefun = false;//鼠标点击在函数widget内
    bool m_memCalbtn; //m+,m-,ms可用
    bool m_memRCbtn;//mr,mc可用
    SciHistoryWidgetTab *m_scihiswidget;
    MemoryPublicTab *m_memoryPublic;
};

#endif // BASICMODULEVER_H
