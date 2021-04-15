#ifndef SCIENTIFICMODULEVER_H
#define SCIENTIFICMODULEVER_H

#include <QStackedWidget>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "tablet/control/textbuttontab.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/control/basickeypadtab.h"
#include "tablet/control/memhiskeypadtab.h"
#include "tablet/control/scientifickeypadtab.h"
#include "tablet/views/memorywidgettab.h"
#include "tablet/memorypublictab.h"
#include "widgets/inputedit.h"
#include "scihistorywidgettab.h"
#include "sciexpressionbartab.h"

class scientificModuleVer : public DWidget
{
    Q_OBJECT
public:
    scientificModuleVer(QWidget *parent = nullptr);
    ~scientificModuleVer();
    void setKeyPress(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void checkLineEmpty();

signals:
    void sciMemTab();
    void clearbtnShow(bool show);
    void changedeg(int deg);

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
    void handleDegChanged();
    void handleFEStateChanged(bool isdown);
    void handlePageStateChanged();
//    void setBasicTabOrder();

private:
    QStackedWidget *m_stackWidget;
    SciExpressionBarTab *m_sciexpressionBar;
    ScientificKeyPadTab *m_scikeypadwidget;
    MemHisKeypadTab *m_memhiskeypad;
    bool m_avail = false;
    bool m_havail = false; //历史记录是否有内容
    int m_deg = 1;//1-deg 2-rad 3-grad
    bool m_FEisdown = false; //FE按键是否被按下
    bool m_Pageisdown = false; //2nd按键是否被按下
    bool m_memCalbtn; //m+,m-,ms
    bool m_memRCbtn;//mr,mc
    SciHistoryWidgetTab *m_scihiswidget;
    MemoryPublicTab *m_memoryPublic;
};

#endif // SCIENTIFICMODULEVER_H
