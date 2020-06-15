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

class SimpleListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SimpleListModel(int mode = 0, QObject *parent = nullptr);//mode:0-标准模式 1-科学模式
    ~SimpleListModel();
    void refrushModel();

    enum Role {
        ExpressionRole = Qt::ToolTipRole,
        ExpressionCount,
        ExpressionPrevious,
        ExpressionNext
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void appendText(const QString &text, bool sci);
    void clearItems();
    void updataList(const QString &text, const int index, bool sci = false);
    void deleteItem(const int index);
    void copyToClipboard(const int index);

signals:
    void updateCount(int);
    void hisbtnhidden();

private:
    QList<QString> m_expressionList;
    bool m_selectedStatus;
    int m_mode; //0-标准模式 1-科学模式
};

#endif
