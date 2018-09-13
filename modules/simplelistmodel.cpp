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

#include "simplelistmodel.h"
#include <QDebug>

SimpleListModel::SimpleListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

SimpleListModel::~SimpleListModel()
{
}

int SimpleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_expressionList.size();
}

QVariant SimpleListModel::data(const QModelIndex &index, int role) const
{
    const int r = index.row();

    switch (role) {
    case ExpressionRole:
        return m_expressionList.at(r);
    case ExpressionCount:
        return m_expressionList.count();
    case ExpressionPrevious:
        return m_expressionList.value(r - 1);
    case ExpressionNext:
        return m_expressionList.value(r + 1);
    }

    return QVariant();
}

void SimpleListModel::appendText(const QString &text)
{
    const QString expression = QString(text).simplified();

    if (m_expressionList.contains(expression)) {
        m_expressionList.removeOne(expression);
    }

    const int size = m_expressionList.size();

    beginInsertRows(QModelIndex(), size, size);
    m_expressionList << expression;
    endInsertRows();
}

void SimpleListModel::clearItems()
{
    beginRemoveRows(QModelIndex(), 0, m_expressionList.size());
    m_expressionList.clear();
    endRemoveRows();
}
