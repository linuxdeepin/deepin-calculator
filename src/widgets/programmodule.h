#ifndef PROGRAMMODULE_H
#define PROGRAMMODULE_H

#include <DWidget>
#include <DGuiApplicationHelper>
#include <DArrowRectangle>
#include <DListWidget>
#include <DMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "views/memorylistwidget.h"
#include "views/programmerarrowdelegate.h"
#include "views/programmeritemwidget.h"
#include "views/prolistview.h"
#include "views/prolistmodel.h"
#include "views/prolistdelegate.h"
#include "control/bitbutton.h"
#include "control/programmerkeypad.h"
#include "control/procheckbtnkeypad.h"
#include "control/prosystemkeypad.h"
#include "widgets/proexpressionbar.h"
#include "widgets/arrowrectangle.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class ProgramModule : public DWidget
{
    Q_OBJECT

public:
    ProgramModule(QWidget *parent = nullptr);
    ~ProgramModule();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void handleCheckBtnKeypadButtonPress(int key);
    void handleKeypadButtonPressByspace(int key);
    //    void shiftArrowListWidgetItemClicked(QListWidgetItem *item);
    //    void byteArrowListWidgetItemClicked(QListWidgetItem *item);
    void shiftArrowListWidgetItemClicked(int row);
    void shiftArrowListWidgetItemSpace();
    void byteArrowListWidgetItemClicked(int row);
    void byteArrowListWidgetItemSpace();
    void checkBtnKeypadThemeChange(int type);
    void radixListChange(const QModelIndex &index, bool isspace);

private:
    void initArrowRectangle();
    void handleEditKeyPress(QKeyEvent *);
    void setwidgetAttribute(bool b);

private:
    ProExpressionBar *m_proExpressionBar;
    ProListView *m_proListView;
    ProListModel *m_proListModel;
    ProListDelegate *m_proListDelegate;
    ProCheckBtnKeypad *m_checkBtnKeypad;
    ProgrammerKeypad *m_programmerKeypad;
    ProSystemKeypad *m_proSystemKeypad;
    QStackedWidget *m_stackWidget;

    ArrowRectangle *m_byteArrowRectangle;
    MemoryListWidget *m_byteArrowListWidget;
    ProgrammerArrowDelegate *m_byteProgrammerArrowDelegate;
    ArrowRectangle *m_shiftArrowRectangle;
    MemoryListWidget *m_shiftArrowListWidget;
    ProgrammerArrowDelegate *m_shiftProgrammerArrowDelegate;

    //rectangle中当前选中的行数
    int m_shiftArrowCurrentRow = 0;
    int m_byteArrowCurrentRow = 0;
};

#endif // PROGRAMMODULE_H
