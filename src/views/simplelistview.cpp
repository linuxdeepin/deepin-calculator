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

#include "simplelistview.h"

#include <QApplication>
#include <QMouseEvent>
#include <QScrollBar>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <DGuiApplicationHelper>

#include "simplelistmodel.h"

SimpleListView::SimpleListView(int mode, QWidget *parent)
    : DListView(parent)
{
    m_mode = mode;

    setVerticalScrollMode(ScrollPerPixel); //鼠标滚轮滚动一次一个像素
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); //设置垂直滚条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked); //单击已选中的内容双击单元格时编辑
//    setFocusPolicy(Qt::TabFocus);
    setAutoScroll(false);
    setSelectionBehavior(QAbstractItemView::SelectRows); //选中一行
    setSelectionMode(QAbstractItemView::SingleSelection); //选中单个目标
    if (m_mode == 0) {
        setFocusPolicy(Qt::NoFocus);
        setFixedHeight(99);
    }

    if (m_mode == 1) {
//        setFocusPolicy(Qt::NoFocus);
        setMouseTracking(true);
    }

    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &SimpleListView::adjustScrollbarMargins);
}

SimpleListView::~SimpleListView()
{
}

/**
 * @brief 添加右键菜单
 */
void SimpleListView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    //缺翻译
    QAction *copy = new QAction(tr("Copy"), menu);
    QAction *clean = new QAction(tr("Delete"), menu);
    menu->addAction(copy);
    menu->addAction(clean);
    connect(copy, &QAction::triggered, this, [ = ]() {
        static_cast<SimpleListModel *>(model())->copyToClipboard(indexAt(event->pos()).row());
    });
    connect(clean, &QAction::triggered, this, [ = ]() {
        static_cast<SimpleListModel *>(model())->deleteItem(indexAt(event->pos()).row());
    });
    if (indexAt(event->pos()).row() >= 0 && m_itemfill) {
        menu->exec(event->globalPos());
    }
    delete menu;
}

/**
 * @brief 给私有变量赋值，历史记录有数据
 */
void SimpleListView::listItemFill(bool itemfill)
{
    m_itemfill = itemfill;
}

void SimpleListView::showTextEditMenuByAltM(const QModelIndex &index)
{
    QMenu *menu = new QMenu(this);
    //缺翻译
    QAction *copy = new QAction(tr("Copy"), menu);
    QAction *clean = new QAction(tr("Delete"), menu);
    menu->addAction(copy);
    menu->addAction(clean);
    connect(copy, &QAction::triggered, this, [ = ]() {
        static_cast<SimpleListModel *>(model())->copyToClipboard(index.row());
    });
    connect(clean, &QAction::triggered, this, [ = ]() {
        static_cast<SimpleListModel *>(model())->deleteItem(index.row());
    });
    if (index.row() >= 0 && m_itemfill) {
        QPoint menupoint;
        menupoint.setX(mapToGlobal(visualRect(index).center()).x());
        menupoint.setY(mapToGlobal(visualRect(index).bottomLeft()).y() - 20);
        menu->exec(menupoint);
    }
    delete menu;
}

/**
 * @brief 更改delegate参数，实现delegate的hover状态
 */
void SimpleListView::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mode == 1 && m_itemfill) {
        if (m_ispressed == true) {
            if (indexAt(m_presspoint) != indexAt(e->pos()))
                m_presschanged = true;
            else
                m_presschanged = false;
            QListView::mouseMoveEvent(e);
        } else {
            static_cast<SimpleListDelegate *>(itemDelegate(indexAt(e->pos())))->paintback(indexAt(e->pos()), 0);
            QWidget::mouseMoveEvent(e);
        }
    } else {
        if (e->x() < width() - 12) {
            QWidget::mouseMoveEvent(e);
        } else {
            QListView::mouseMoveEvent(e);
        }
    }
}

/**
 * @brief 设置滚动区域边距
 */
void SimpleListView::adjustScrollbarMargins()
{
    if (!isVisible()) {
        return;
    }
    QEvent event(QEvent::LayoutRequest);
    QApplication::sendEvent(this, &event);

    if (!verticalScrollBar()->visibleRegion().isEmpty()) {
        setViewportMargins(0, 0, -verticalScrollBar()->sizeHint().width(), 0);
    } else {
        setViewportMargins(0, 0, 0, 0);
    }
}

/**
 * @brief 更改delegate参数，实现delegate的press状态
 */
void SimpleListView::mousePressEvent(QMouseEvent *event)
{
    static_cast<SimpleListModel *>(model())->refrushModel();
    if (event->buttons() & Qt::LeftButton) {
        if (indexAt(event->pos()).row() > -1) {
            m_presspoint = event->pos();
            if (m_mode == 1)
                static_cast<SimpleListDelegate *>(itemDelegate(indexAt(m_presspoint)))->paintback(indexAt(m_presspoint), 2);
            m_currentrow = indexAt(m_presspoint).row();
            m_ispressed = true;
        }
    }
//    DListView::mousePressEvent(event);
}

/**
 * @brief 更改delegate参数，实现delegate的release状态
 */
void SimpleListView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mode == 1) {
        static_cast<SimpleListModel *>(model())->refrushModel();
        if (m_currentrow == indexAt(event->pos()).row() && m_currentrow >= 0) {
            emit obtainingHistorical(indexAt(event->pos()));
            static_cast<SimpleListDelegate *>(itemDelegate(indexAt(m_presspoint)))->paintback(indexAt(m_presspoint), 1);
        } else {
            if (m_presschanged == true) {
                static_cast<SimpleListDelegate *>(itemDelegate(indexAt(event->pos())))->paintback(indexAt(event->pos()), 1);
            } else
                static_cast<SimpleListDelegate *>(itemDelegate(indexAt(m_presspoint)))->paintback(indexAt(m_presspoint), 0);
        }
    } else if (m_mode == 0) {
        if (m_currentrow == indexAt(event->pos()).row() && m_currentrow >= 0) {
            emit obtainingHistoricalSimple(indexAt(event->pos()));
        }
    }
    m_ispressed = false;
    m_presspoint = QPoint();
    DListView::mouseReleaseEvent(event);
}

void SimpleListView::keyPressEvent(QKeyEvent *e)
{
    bool ispressalt = e->modifiers() == Qt::AltModifier;
    switch (e->key()) {
    case Qt::Key_Up:
        if (currentIndex().row() > 0) {
            setCurrentIndex(this->model()->index(currentIndex().row() - 1, 0));
            static_cast<SimpleListDelegate *>(itemDelegate(currentIndex()))->currentfocusindex(currentIndex());
            m_currentindexrow = currentIndex().row();
            scrollTo(this->model()->index(currentIndex().row(), 0));
        }
        break;
    case Qt::Key_Down:
        if (currentIndex().row() < this->count() - 1) {
            setCurrentIndex(this->model()->index(currentIndex().row() + 1, 0));
            static_cast<SimpleListDelegate *>(itemDelegate(currentIndex()))->currentfocusindex(currentIndex());
            m_currentindexrow = currentIndex().row();
            scrollTo(this->model()->index(currentIndex().row(), 0));
        }
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit obtainingHistorical(currentIndex());
        break;
    case Qt::Key_M:
        if (ispressalt)
            showTextEditMenuByAltM(currentIndex());
        break;
    default:
        DListView::keyPressEvent(e);
        break;
    }
}

/**
 * @brief 焦点进入时设置焦点在第一条历史记录
 */
void SimpleListView::focusInEvent(QFocusEvent *event)
{
    setCurrentIndex(this->model()->index(m_currentindexrow, 0));
    static_cast<SimpleListDelegate *>(itemDelegate(currentIndex()))->currentfocusindex(currentIndex());
    scrollTo(this->model()->index(0, 0));
    DListView::focusInEvent(event);
}
