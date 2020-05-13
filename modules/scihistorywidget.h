#ifndef SCIHISTORYWIDGET_H
#define SCIHISTORYWIDGET_H


#include <DWidget>
#include <DGuiApplicationHelper>
#include <QStackedLayout>
#include <QGridLayout>
#include <DButtonBox>
#include <DIconButton>

#include "../widgets/textbutton.h"
#include "../widgets/iconbutton.h"
#include "../widgets/basickeypad.h"
#include "../widgets/memorykeypad.h"
#include "../widgets/tricombobox.h"
#include "expressionbar.h"
#include "memorywidget.h"
#include "./widgets/scibasickeypad.h"
#include "../widgets/inputedit.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class SciHistoryWidget : public DWidget
{
    Q_OBJECT

public:
    SciHistoryWidget(QWidget *parent = nullptr);
    ~SciHistoryWidget();

public slots:

private:

protected:
    //void paintEvent(QPaintEvent *);

private:
    QStackedLayout *m_stacklayout;
    MemoryWidget *m_memorywidget;
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    DButtonBox *m_buttonbox;
    DIconButton *m_clearbutton;
};

#endif // SCIHISTORYWIDGET_H
