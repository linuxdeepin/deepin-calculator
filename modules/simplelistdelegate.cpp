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

#include "simplelistdelegate.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include "dthememanager.h"
#include "simplelistmodel.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

SimpleListDelegate::SimpleListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    m_selected = false;
    m_simpleListDelegate = this;
}

SimpleListDelegate::~SimpleListDelegate()
{
    delete m_simpleListDelegate;
    m_simpleListDelegate = NULL;
}

void SimpleListDelegate::setHisLink(const int link)
{
    m_linkItem.push_back(link);
}

void SimpleListDelegate::setHisLinked(const int linked)
{
    m_linkedIten.push_back(linked);
}

void SimpleListDelegate::removeLine(const int link, const int linked)
{
    for (int i = 0; i < m_linkItem.size(); ++i) {
        if (m_linkItem[i] == link)
            m_linkItem.remove(i);
    }
    for (int i = 0; i < m_linkedIten.size(); ++i) {
        if (m_linkedIten[i] == linked)
            m_linkedIten.remove(i);
    }
}

void SimpleListDelegate::removeLine(int index)
{
    if (index < m_linkItem.size())
        m_linkItem.remove(index);
    if (index < m_linkedIten.size())
        m_linkedIten.remove(index);
}

void SimpleListDelegate::removeHisLink()
{
    if (!m_linkItem.isEmpty())
        m_linkItem.removeLast();
}

void SimpleListDelegate::removeAllLink()
{
    m_linkedIten.clear();
    m_linkItem.clear();
}

void SimpleListDelegate::removeHisLinked()
{
    if (!m_linkedIten.isEmpty())
        m_linkedIten.removeLast();
}

void SimpleListDelegate::setThemeType(int type)
{
    m_type = type;
}

void SimpleListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    const QString expression = index.data(SimpleListModel::ExpressionRole).toString();
    QRect rect(option.rect);
    rect.setRight(321);
    const int padding = 15;
    QString errorFontColor;
    QString fontColor;
    QString linkColor;
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);

    QFont font;
    for (int i = 16; i > 12; --i) {  // edit for bug-17238
        font.setPixelSize(i);

        QFontMetrics fm(font);
        int fontWidth = fm.width(expression);
        int editWidth = rect.width() - 24;

        if (fontWidth < editWidth)
            break;
    }
    painter->setFont(font);

    QStringList splitList = expression.split("＝");
    QString resultStr = splitList.last();
    int resultWidth = painter->fontMetrics().width(resultStr);

    if (resultWidth > rect.width() / 1.4) {
        resultStr = painter->fontMetrics().elidedText(resultStr, Qt::ElideRight,
                                                      rect.width() / 1.4 + padding);
        resultWidth = painter->fontMetrics().width(resultStr);
    }

    if (m_type == 1) {
        errorFontColor = "#FF5736";  //edit for bug-21508
        linkColor = "#3489DF";
        fontColor = "#838483";
    } else {
        errorFontColor = "#9A2F2F";  //edit for bug-21508
        linkColor = "#3489DF";
        fontColor = "#838483";
    }

    // check result text is error.
    if (resultStr == tr("Expression error")) {
        painter->setPen(QColor(errorFontColor));
    } else {
        painter->setPen(QColor(fontColor));
        for (int i = 0; i < m_linkItem.size(); ++i) {
            if (m_linkItem[i] == index.row())
                painter->setPen(QColor(linkColor));
        }
    }

    int equalStrWidth = painter->fontMetrics().width(" ＝ ");
    QString expStr = painter->fontMetrics().elidedText(
                         splitList.first(), Qt::ElideLeft, rect.width() - resultWidth - padding * 2 - equalStrWidth);
    // QString expStr = splitList.first();

    if (m_selected) {
        //edit for bug--21508
        QRect resultRect(rect.topRight().x() - resultWidth - padding - 2, rect.y() + 3, resultWidth + 7,
                         rect.height() - 6);
        QPainterPath path;
        path.addRoundedRect(resultRect, 4, 4);

        // Setting Gradient
//        QLinearGradient linear(resultRect.topLeft(), resultRect.bottomRight());
//        linear.setColorAt(0, QColor(0, 151, 231, 255));
//        linear.setColorAt(1, QColor(0, 122, 219, 255));
//        linear.setColorAt(0, QColor("#0081FF"));
//        linear.setColorAt(1, QColor("#0081FF"));
//        linear.setSpread(QGradient::PadSpread);
        //edit for bug--21476
        if (m_type == 1) {
            QBrush brush(QColor("#0081FF"));
            painter->fillPath(path, brush);
        } else {
            QBrush brush(QColor("#0059D2"));
            painter->fillPath(path, brush);
        }

        painter->setPen(QPen(QColor(Qt::white)));
        m_simpleListDelegate->setSelect(false);
    }

    if (splitList.size() == 1) {
        // draw expression text;
        painter->setPen(QColor(fontColor));
        painter->drawText(
            QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()),
            Qt::AlignVCenter | Qt::AlignRight, expStr);
    } else {
        // draw result text.
        painter->drawText(
            QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()),
            Qt::AlignVCenter | Qt::AlignRight, resultStr);

        QString linkNum, exp;
        m_simpleListDelegate->cutApart(expStr, linkNum, exp);
        exp = exp + " ＝ ";

        // draw expression text;
        painter->setPen(QColor(fontColor));
        painter->drawText(QRect(rect.x() + padding, rect.y(),
                                rect.width() - resultWidth - padding * 2, rect.height()),
                          Qt::AlignVCenter | Qt::AlignRight, exp);

        painter->setPen(QColor(fontColor));
        for (int i = 0; i < m_linkedIten.size(); ++i) {
            if (m_linkedIten[i] == index.row())
                painter->setPen(QColor(linkColor));
        }

        int expWidth = painter->fontMetrics().width(exp);
        painter->drawText(QRect(rect.x() + padding, rect.y(),
                                rect.width() - resultWidth - expWidth - padding * 2, rect.height()),
                          Qt::AlignVCenter | Qt::AlignRight, linkNum);
    }
}

QSize SimpleListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    return QSize(-1, 35);
}

bool SimpleListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                     const QStyleOptionViewItem &option, const QModelIndex &index)
{
    m_selected = true;
    emit obtainingHistorical(index);
    return true;
}

void SimpleListDelegate::cutApart(const QString text, QString &linkNum, QString &expStr)
{
    QString exp = text;
    QStringList list;
    int index = 0;
    list = exp.split(QRegExp("[＋－×÷()]"), QString::SkipEmptyParts);
    if (list.isEmpty() || list.size() == 1) {
        linkNum = "";
        expStr = exp;
        return;
    }
    if (exp.at(0) != "－") {
        while (exp.at(index) == "(") {
            ++index;
            linkNum.append("(");
            if (exp.at(index) == "－")
                linkNum.append("－");
        }
        linkNum.append(list.at(0));
    } else {
        linkNum.append("－");
        if (exp.at(1) == "(")
            linkNum.append("(");
        linkNum.append(list.at(0));
    }
    if (linkNum.at(linkNum.size() - 1) == "e")
        linkNum = linkNum + exp.at(exp.indexOf("e") + 1) + list.at(1);
    expStr = text.right(text.length() - linkNum.length());
}
