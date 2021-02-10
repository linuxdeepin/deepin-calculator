/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIMPLELISTMODEL_H
#define SIMPLELISTMODEL_H

#include <QAbstractListModel>

#include "../../3rdparty/core/evaluator.h"

class SimpleListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SimpleListModel(int mode = 0, QObject *parent = nullptr);//mode:0-标准模式 1-科学模式
    ~SimpleListModel();
    void refrushModel();

    //model的data数据类型
    enum Role {
        ExpressionWithOutTip = Qt::DisplayRole,//无提示文本
        ExpressionRole = Qt::ToolTipRole, //有提示文本
        ExpressionCount = 4, //数据数量
        ExpressionPrevious, //前一个数据
        ExpressionNext, //下一个数据
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void appendText(const QString &text, bool sci);
    void clearItems();
    void updataList(const QString &text, const int index, bool sci = false);
    void updataList(Quantity ans, const QString &text, const int index);
    void deleteItem(const int index);
    void copyToClipboard(const int index);
    Quantity getAnswer(const int index);
    //进制切换
    void radixChanged(int baseori, int basedest);
    bool isNumber(QChar a);//判断是否为数字(分隔符)
    QString formatExpression(const int &probase, const QString &text);
    void answerOutOfRange(Quantity ans);
    void expOutofRange();

signals:
    void hisbtnhidden();

private:
    QList<QString> m_expressionList;
    bool m_selectedStatus;
    int m_mode = 0; //0-标准模式 1-科学模式
    QList<Quantity> m_answerlist; //对应历史记录的answer

    //进制切换时用于替换所有数字
    QVector<QString> m_numvec;
    QVector<QString> m_opvec;
    QString m_textorder;
    bool m_numchanged = false;
};

#endif
