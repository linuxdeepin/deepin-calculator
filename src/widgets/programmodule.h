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

#include "src/views/programmerarrowdelegate.h"
#include "src/views/programmeritemwidget.h"
#include "src/views/prolistview.h"
#include "src/views/prolistmodel.h"
#include "src/views/prolistdelegate.h"
#include "src/control/bitbutton.h"
#include "src/control/programmerkeypad.h"
#include "src/control/procheckbtnkeypad.h"
#include "src/control/prosystemkeypad.h"
#include "src/widgets/proexpressionbar.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class ProgramModule : public DWidget
{
    Q_OBJECT

public:
    ProgramModule(QWidget *parent = nullptr);
    ~ProgramModule();

private:
    ProExpressionBar *m_proExpressionBar;
    ProListView *m_proListView;
    ProListModel *m_proListModel;
    ProListDelegate *m_proListDelegate;
    ProCheckBtnKeypad *m_checkBtnKeypad;
    ProgrammerKeypad *m_programmerKeypad;
    ProSystemKeypad *m_proSystemKeypad;
    QStackedWidget *m_stackWidget;

    DArrowRectangle *m_arrowRectangle;
    DListWidget *m_arrowListWidget;
    ProgrammerArrowDelegate *m_programmerArrowDelegate;
};

#endif // PROGRAMMODULE_H
