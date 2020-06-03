#ifndef SCIEXPRESSIONBAR_H
#define SCIEXPRESSIONBAR_H

#include <QKeyEvent>
#include <QPair>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include <DWidget>

#include "../core/evaluator.h"
#include "../widgets/inputedit.h"
#include "simplelistdelegate.h"
#include "simplelistmodel.h"
#include "simplelistview.h"

DWIDGET_USE_NAMESPACE

//struct historicalLinkageIndex

//{
//    int linkageTerm;
//    int linkedItem;
//    QString linkageValue;
//    bool isLink;
//    historicalLinkageIndex()
//    {
//        linkageTerm = -1;
//        linkedItem = -1;
//        isLink = false;
//    }
//};

struct strcksci {
    QString text;
    bool isResult;
    strcksci() { isResult = false; }
};

class SciExpressionBar : public DWidget
{
    Q_OBJECT

public:
    SciExpressionBar(QWidget *parent = nullptr);
    ~SciExpressionBar();

    void setContinue(bool isContinue);

signals:
    void keyPress(QKeyEvent *);
    void clearStateChanged(bool);
    void themeChange(int type);
    void turnDeg();
    void fEStateChanged(bool);

public slots:
    void enterNumberEvent(const QString &text);
    void enterSymbolEvent(const QString &text);
    void enterPointEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    void enterPercentEvent();
    void enterBracketsEvent();
    void enterLeftBracketsEvent();
    void enterRightBracketsEvent();
    void enterDeleteEvent();
    void entereEvent();
    void enterExpEvent(int mod);
    void enterSinEvent();
    void enterFEEvent(bool isdown);
    void moveLeft();
    void moveRight();
    void copyResultToClipboard();
    void copyClipboard2Result();
    void allElection();
    void shear();
    void computationalResults(const QString &expression, QString &result);
//    void historicalLinkage(int index, QString newValue);
//    void clearLinkageCache(const QString &text, bool isequal);
//    void setLinkState(const QModelIndex index);
//    void settingLinkage(const QModelIndex &index);
//    void settingLinkage();
    void Undo();
    void addUndo();
    void Redo();
    void initTheme(int type);
    void clearSelection();
    void setSelection();
    void getSelection();
    void setResultFalse();
    void replaceSelection(QString text);
    InputEdit *getInputEdit();

private slots:
    void handleTextChanged(const QString &text);
    void revisionResults(const QModelIndex &index);

private:
    bool cursorPosAtEnd();
    QString formatExpression(const QString &text);
    QString completedBracketsCalculation(QString &text);
    bool isOperator(const QString &text);
//    bool cancelLink(int index);
//    void judgeLinkageAgain();
    void initConnect();
    QString symbolComplement(const QString exp);
    QString pasteFaultTolerance(QString exp);
    QString pointFaultTolerance(const QString &text);
    void clearSelectSymbol();
    void expressionCheck();

private:
    Evaluator *m_evaluator;
//    SimpleListView *m_listView;
//    SimpleListDelegate *m_listDelegate;
//    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;
    QLineEdit *m_lineEdit;

    QString m_unfinishedExp;  //未完成表达式
    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;           //计算结果
    bool m_isAutoComputation;  //自动计算
    bool m_inputNumber;        //输入数字
    bool m_isUndo;
//    int m_hisRevision;  //历史记录修改

//    bool m_isLinked;     //联动状态
//    int m_linkageIndex;  //联动索引缓存
//    int m_Selected;      //选中
    QString m_selection;
//    QVector<historicalLinkageIndex> m_hisLink;  //历史联动索引
    QVector<QString> m_undo;
    QVector<QString> m_redo;

    bool m_meanexp;    //是否表达式为纯数字
};
#endif // SCIEXPRESSIONBAR_H
