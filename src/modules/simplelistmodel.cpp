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
#include <QClipboard>
#include <QApplication>

SimpleListModel::SimpleListModel(int mode, QObject *parent)
    : QAbstractListModel(parent)
{
    m_selectedStatus = false;
    if (mode == 1) {
        m_mode = mode;
        clearItems();
    }
}

SimpleListModel::~SimpleListModel() {}

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

void SimpleListModel::appendText(const QString &text, bool sci)
{
    auto expression = text.simplified();

    /*if (m_expressionList.contains(expression)) {
        m_expressionList.removeOne(expression);
    }*/

    const int size = m_expressionList.size();

    if (sci) {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_expressionList.removeAt(0);
        m_expressionList.insert(0, expression);
        endRemoveRows();
    } else {
        beginInsertRows(QModelIndex(), size, size);
        m_expressionList << expression;
        endInsertRows();
    }
}

void SimpleListModel::clearItems()
{
    beginRemoveRows(QModelIndex(), 0, m_expressionList.size());
    m_expressionList.clear();
    endRemoveRows();
    answerlist.clear();
    if (m_mode == 1) {
        beginInsertRows(QModelIndex(), 0, 0);
        //缺翻译
        m_expressionList << "历史记录中没有数据";
        endInsertRows();
    }
}

void SimpleListModel::refrushModel()
{
    beginResetModel();
    endResetModel();

    emit updateCount(this->rowCount(QModelIndex()));
}

void SimpleListModel::updataList(const QString &text, const int index, bool sci)
{
    if (m_expressionList.count() == 500) {
        if (sci)
            deleteItem(499);
        else
            deleteItem(0);
    }
    QString exp = text;
    exp = exp.replace('+', QString::fromUtf8("＋"))
          .replace('-', QString::fromUtf8("－"))
          .replace('*', QString::fromUtf8("×"))
          .replace('/', QString::fromUtf8("÷"))
          //.replace('x', QString::fromUtf8("×"))
          .replace('X', QString::fromUtf8("×"));
    if (exp.indexOf("x") != -1) {
        if (exp.at(exp.indexOf("x") - 1) != "E")
            exp = exp.replace('x', QString::fromUtf8("×"));
    }

    if (index == -1) {
        appendText(exp, sci);
    } else {
        if (sci) {
            beginInsertRows(QModelIndex(), index, index);
            m_expressionList.insert(index, exp);
            endInsertRows();
        } else {
            beginRemoveRows(QModelIndex(), index, index);
            m_expressionList.removeAt(index);
            m_expressionList.insert(index, exp);
            endRemoveRows();
        }
    }
}

void SimpleListModel::updataList(Quantity ans, const QString &text, const int index)
{
    if (m_expressionList.count() == 500) {
        deleteItem(499);
        answerlist.pop_back();

    }
    QString exp = text;
    exp = exp.replace('+', QString::fromUtf8("＋"))
          .replace('-', QString::fromUtf8("－"))
          .replace('*', QString::fromUtf8("×"))
          .replace('/', QString::fromUtf8("÷"))
          //.replace('x', QString::fromUtf8("×"))
          .replace('X', QString::fromUtf8("×"));
    if (exp.indexOf("x") != -1) {
        if (exp.at(exp.indexOf("x") - 1) != "E")
            exp = exp.replace('x', QString::fromUtf8("×"));
    }
    beginInsertRows(QModelIndex(), index, index);
    m_expressionList.insert(index, exp);
    endInsertRows();
    answerlist.insert(0, ans);
}

void SimpleListModel::deleteItem(const int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_expressionList.removeAt(index);
    endRemoveRows();
    if (answerlist.count() > 0)
        answerlist.removeAt(index);
    if (m_expressionList.count() == 0) {
        emit hisbtnhidden();
    }
}

void SimpleListModel::copyToClipboard(const int index)
{
    QClipboard *clipboard = QApplication::clipboard();
    QString copy = m_expressionList.at(index);
    clipboard->setText(copy.replace(" ", ""));
}

Quantity SimpleListModel::getAnswer(const int index)
{
    if (answerlist.count() > index)
        return answerlist.at(index);
    return Quantity();
}
