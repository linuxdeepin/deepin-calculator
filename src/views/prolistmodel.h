/*
* Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
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

#ifndef PROLISTMODEL_H
#define PROLISTMODEL_H

#include <QAbstractListModel>
#include <QClipboard>
#include <QApplication>

#include "../../3rdparty/core/evaluator.h"

class ProListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ProListModel(QObject *parent = nullptr);
    ~ProListModel();
    void refrushModel();

    //model的data数据类型
    enum Role {
        ExpressionRole = Qt::DisplayRole, //无提示文本
        ExpressionCount = 4, //数据数量
        ExpressionPrevious, //前一个数据
        ExpressionNext //下一个数据
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void clearItems();
    void updataList(const Quantity ans);
    void copyToClipboard(const int index);

private:
    QList<QString> m_expressionList;
};

#endif // PROLISTMODEL_H
