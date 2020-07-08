/*
 * 1. @类名:    SciExpressionBar
 * 2. @作者:    夏菁 ut000489
 * 3. @日期:    2020-07-01
 * 4. @说明:    科学计算器表达式计算函数
 */
#ifndef SCIEXPRESSIONBAR_H
#define SCIEXPRESSIONBAR_H

#include <QKeyEvent>
#include <QPair>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QToolTip>

#include <DWidget>

#include "src/core/evaluator.h"
#include "src/widgets/inputedit.h"
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
    QPair<bool, QString> getexpression();
    Quantity getanswer();

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
    void enterDegEvent(int mod);
    void enterSinEvent();
    void enterFEEvent(bool isdown);
    void enterPIEvent();
    void enterEulerEvent();
    void enterModEvent();
    void enterx2Event();
    void enterDerivativeEvent();
    void enterFactorialsEvent();
    void enterExpEvent();
    void enterCosEvent();
    void enterx3Event();
    void enterTanEvent();
    void enterxyEvent();
    void enterCotEvent();
    void enter10xEvent();
    void enterModulusEvent();
    void enterLogEvent();
    void enterRandEvent();
    void enterLnEvent();
    void enterArcsinEvent();
    void enterSqrtEvent();
    void enterArccosEvent();
    void enterCbrtEvent();
    void enterArctanEvent();
    void enterYrootEvent();
    void enterArccotEvent();
    void enter2xEvent();
    void enterlogyxEvent();
    void enterexEvent();
    void enterOppositeEvent();
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
    void revisionResults(const QModelIndex &index);
    void hisRevisionResults(const QModelIndex &index, Quantity ans);

private slots:
    void handleTextChanged(const QString &text);


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
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    QString m_unfinishedExp;  //未完成表达式
    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;           //计算结果
    bool m_isAutoComputation;  //自动计算
    bool m_inputNumber;        //输入数字
    bool m_isUndo;
    QString m_selection;
    QVector<QString> m_undo;
    QVector<QString> m_redo;

    bool m_meanexp;    //是否表达式为纯数字
    bool m_FEisdown = false; //科学计算开关打开
    //支持的功能列表
    QList<QString> m_funclist;
    QString m_expression = QString();
    QPair<bool, QString> m_pair; //getexpression()函数的返回值，first为是否是错误表达式，second为表达式
    Quantity m_listanswer = Quantity(); //历史记录对应的answer
};
#endif // SCIEXPRESSIONBAR_H
