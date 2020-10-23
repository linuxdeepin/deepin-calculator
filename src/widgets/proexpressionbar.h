#ifndef PROEXPRESSIONBAR_H
#define PROEXPRESSIONBAR_H

#include <QApplication>
#include <QClipboard>

#include "widgets/inputedit.h"
#include "views/simplelistview.h"
#include "views/simplelistmodel.h"
#include "views/simplelistdelegate.h"

DWIDGET_USE_NAMESPACE

class ProExpressionBar : public DWidget
{
    Q_OBJECT

public:
    ProExpressionBar(QWidget *parent = nullptr);
    ~ProExpressionBar();
    void mouseMoveEvent(QMouseEvent *event);
    InputEdit *getInputEdit();

signals:
//    void keyPress(QKeyEvent *);
    void clearStateChanged(bool);

public slots:
    void enterEqualEvent();
    void initTheme(int type);
    void revisionResults(const QModelIndex &index);
    void addUndo();
    void copyResultToClipboard();
    void copyClipboard2Result();
    void allElection();
    void shear();
    void deleteText();
    void Undo();
    void Redo();
    void setResultFalse();

private:
    void initConnect();
    void replaceSelection(QString text);
    QString symbolComplement(const QString exp);
    QString pointFaultTolerance(const QString &text);

private slots:
    void handleTextChanged();

private:
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;           //计算结果
    bool m_isUndo;
    QVector<QString> m_undo;
    QVector<QString> m_redo;
};

#endif // PROEXPRESSIONBAR_H
