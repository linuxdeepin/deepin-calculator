#ifndef SCIHISTORYWIDGET_H
#define SCIHISTORYWIDGET_H
/*******************************************************************************
 1. @类名:    SciHistoryWidget
 2. @作者:    京洲
 3. @日期:    2020-07-01
 4. @说明:    科学计算器历史记录界面
*******************************************************************************/

#include <DWidget>
#include <DGuiApplicationHelper>
#include <QStackedLayout>
#include <QGridLayout>
#include <DButtonBox>
#include <DIconButton>
#include <QStackedWidget>

#include "../widgets/textbutton.h"
#include "../widgets/iconbutton.h"
#include "../widgets/basickeypad.h"
#include "../widgets/memorykeypad.h"
#include "../widgets/tricombobox.h"
#include "expressionbar.h"
#include "memorywidget.h"
#include "memorypublic.h"
#include "../widgets/scibasickeypad.h"
#include "../widgets/inputedit.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class SciHistoryWidget : public DWidget
{
    Q_OBJECT
public:
    enum memOperate {
        generateData, memoryplus, memoryminus, memoryclean,
        widgetplus, widgetminus
    };
    SciHistoryWidget(QWidget *parent = nullptr);
    ~SciHistoryWidget();
//    void paintEvent(QPaintEvent *event);
    void focusOnButtonbox();
    MemoryWidget *getMemoryWidget();
    void memoryFunctions(memOperate operate = generateData, Quantity answer = Quantity(), int row = -1);

    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
public slots:
    void themeChanged(int type);
    void ishistoryfilled(bool b);

private:

protected:
    //void paintEvent(QPaintEvent *);

private:
    QStackedLayout *m_stacklayout;
    MemoryWidget *m_memorywidget;
    DButtonBox *m_buttonbox;
    IconButton *m_clearbutton; //历史界面清除按钮
    IconButton *m_clearbuttonM; //内存界面清除按钮
    MemoryPublic *memoryPublic;
    QStackedWidget *m_buttonstack;
    //0-hide 1-show,hide-是否为空,index-当前是历史界面/内存界面
    bool m_ishideM = false;
    bool m_indexM = false;
    bool m_ishideH = false;
    bool m_indexH = true;
};

#endif // SCIHISTORYWIDGET_H
