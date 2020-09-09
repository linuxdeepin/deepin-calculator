#ifndef PROLISTMODEL_H
#define PROLISTMODEL_H

#include <QAbstractListModel>
#include <QClipboard>
#include <QApplication>

#include "src/core/evaluator.h"

class ProListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ProListModel(QObject *parent = nullptr);
    ~ProListModel();
    void refrushModel();

    //model的data数据类型
    enum Role {
        ExpressionRole = Qt::DisplayRole, //无提示文本
        ExpressionCount, //数据数量
        ExpressionPrevious, //前一个数据
        ExpressionNext //下一个数据
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void clearItems();
    void updataList(const QString &text, const int index);
    void copyToClipboard(const int index);

private:
    QList<QString> m_expressionList;
};

#endif // PROLISTMODEL_H
