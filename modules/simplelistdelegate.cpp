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
#include "simplelistmodel.h"
#include "dthememanager.h"
#include "utils.h"
#include <QPainter>
#include <QDebug>

DWIDGET_USE_NAMESPACE

SimpleListDelegate::SimpleListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{

}

SimpleListDelegate::~SimpleListDelegate()
{
}

void SimpleListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString expression = index.data(SimpleListModel::ExpressionRole).toString();
    const QString theme = DThemeManager::instance()->theme();
    const QRect rect(option.rect);
    const int padding = 15;
    QString errorFontColor;
    QString fontColor;
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QFont font;
    font.setPointSize(11);
    painter->setFont(font);

    QStringList splitList = expression.split("＝");
    QString resultStr = splitList.last();
    int resultWidth = painter->fontMetrics().width(resultStr);

    if (resultWidth > rect.width() / 1.4) {
        resultStr = painter->fontMetrics().elidedText(resultStr, Qt::ElideRight, rect.width() / 1.4 + padding);
        resultWidth = painter->fontMetrics().width(resultStr);
    }

    // init color.
    if (theme == "light") {
        errorFontColor = "#F37D54";
        fontColor = "#636363";
    } else {
        errorFontColor = "#F37D54";
        fontColor = "#C3C3C3";
    }

    // check result text is error.
    if (resultStr == tr("Expression Error")) {
        painter->setPen(QColor(errorFontColor));
    } else {
        painter->setPen(QColor(fontColor));
    }

    int equalStrWidth = painter->fontMetrics().width(" ＝ ");
    QString expStr = painter->fontMetrics().elidedText(splitList.first(), Qt::ElideLeft,
                                                       rect.width() - resultWidth - padding * 2 - equalStrWidth);

    // draw result text.
    painter->drawText(QRect(rect.x() + padding,
                            rect.y(), rect.width() - padding * 2, rect.height()),
                      Qt::AlignVCenter | Qt::AlignRight, resultStr);

    // draw expression text;
    painter->setPen(QColor(fontColor));
    painter->drawText(QRect(rect.x() + padding,
                            rect.y(), rect.width() - resultWidth - padding * 2, rect.height()),
                      Qt::AlignVCenter | Qt::AlignRight, expStr + " ＝ ");
}

QSize SimpleListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(-1, 35);
}
