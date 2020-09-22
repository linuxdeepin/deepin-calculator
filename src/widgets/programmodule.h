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

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class ProgramModule : public DWidget
{
    Q_OBJECT

public:
    ProgramModule(QWidget *parent = nullptr);
    ~ProgramModule();

private slots:
    void handleCheckBtnKeypadButtonPress(int key);
    void shiftArrowListWidgetItemClicked(QListWidgetItem *item);
    void byteArrowListWidgetItemClicked(QListWidgetItem *item);
    void checkBtnKeypadThemeChange(int type);

private:
    void initArrowRectangle();
    void handleEditKeyPress(QKeyEvent *);

private:
    ProExpressionBar *m_proExpressionBar;
    ProListView *m_proListView;
    ProListModel *m_proListModel;
    ProListDelegate *m_proListDelegate;
    ProCheckBtnKeypad *m_checkBtnKeypad;
    ProgrammerKeypad *m_programmerKeypad;
    ProSystemKeypad *m_proSystemKeypad;
    QStackedWidget *m_stackWidget;

    DArrowRectangle *m_byteArrowRectangle;
    DListWidget *m_byteArrowListWidget;
    ProgrammerArrowDelegate *m_byteProgrammerArrowDelegate;
    DArrowRectangle *m_shiftArrowRectangle;
    DListWidget *m_shiftArrowListWidget;
    ProgrammerArrowDelegate *m_shiftProgrammerArrowDelegate;
};

#endif // PROGRAMMODULE_H
