#ifndef PROEXPRESSIONBAR_H
#define PROEXPRESSIONBAR_H


#include "src/widgets/inputedit.h"
#include "src/views/simplelistview.h"
#include "src/views/simplelistmodel.h"
#include "src/views/simplelistdelegate.h"

DWIDGET_USE_NAMESPACE

class ProExpressionBar : public DWidget
{
    Q_OBJECT

public:
    ProExpressionBar(QWidget *parent = nullptr);
    ~ProExpressionBar();

public slots:
    void initTheme(int type);

private:
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;
};

#endif // PROEXPRESSIONBAR_H
