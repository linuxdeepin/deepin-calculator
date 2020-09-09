#ifndef PROGRAMMODULE_H
#define PROGRAMMODULE_H

#include <DWidget>
#include <DGuiApplicationHelper>
#include <DArrowRectangle>
#include <DListWidget>
#include <DMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "src/views/programmerarrowdelegate.h"
#include "src/views/programmeritemwidget.h"
#include "src/views/prolistview.h"
#include "src/views/prolistmodel.h"
#include "src/views/prolistdelegate.h"
#include "src/control/bitbutton.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class ProgramModule : public DWidget
{
    Q_OBJECT

public:
    ProgramModule(QWidget *parent = nullptr);
    ~ProgramModule();

private:
    DArrowRectangle *m_arrowRectangle;
    DListWidget *m_arrowListWidget;
    ProgrammerArrowDelegate *m_programmerArrowDelegate;
};

#endif // PROGRAMMODULE_H
