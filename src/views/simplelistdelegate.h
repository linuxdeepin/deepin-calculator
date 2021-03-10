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

#ifndef SIMPLELISTDELEGATE_H
#define SIMPLELISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE
class SimpleListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    Q_PROPERTY(bool m_selected READ isSelected WRITE setSelect)
public:
    SimpleListDelegate(int mode, QObject *parent = nullptr);//mode:0-标准模式 1-科学模式
    ~SimpleListDelegate();
    void setHisLink(const int link);
    void setHisLinked(const int linked);
    void removeLine(const int link, const int linked);
    void removeLine(int index);
    void removeHisLink();
    void removeAllLink();
    void removeHisLinked();  // add 20200318 for fix cleanlinkcache
    bool isSelected() const {return m_selected;}
    void setSelect(bool isSelect) { m_selected = isSelect; }

public slots:
    void setThemeType(int type);
    void paintback(const QModelIndex &index, int state);//state 0-normal 1-hover 2-press
    void currentfocusindex(QModelIndex index);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    void drawFocusStatus(QPainter *painter, const QStyleOptionViewItem &option) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
//                     const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    void obtainingHistorical(const QModelIndex &index);
    void historicalLinkage(const QModelIndex &index);

private:
    void cutApart(const QString text, QString &linkNum, QString &expStr);

    SimpleListDelegate *m_simpleListDelegate;
    QVector<int> m_linkItem;
    QVector<int> m_linkedIten;
    bool m_selected;
    int m_type = 0;
    int m_mode = 0;
    int m_row = -1;
    int m_state = 0;
    QModelIndex m_focusindex;
};

#endif
