/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* Maintainer: xiajing <xiajing@uniontech.com>
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

#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QList>
#include <QObject>
#include <DPushButton>

#include "src/core/evaluator.h"
#include "src/widgets/iconbutton.h"
#include "memoryitemwidget.h"
#include "memorylistwidget.h"
#include "memoryitemdelegate.h"

/**
 * @brief 标准模式内存界面
 */
class MemoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(int mode = 0, QWidget *parent = nullptr);
    ~MemoryWidget();

    void generateData(Quantity answer);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    bool event(QEvent *event);

    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();//内存列表为空时插入一个item
    QPair<QString, Quantity> getfirstnumber();//MR

    void widgetplusslot(int row, Quantity answer);
    void widgetminusslot(int row, Quantity answer);
    bool isWidgetEmpty(); //内存列表是否为空

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void insidewidget();
    void mListUnavailable();
    void mListAvailable();
    void itemclick(const QPair<QString, Quantity>);
    void themechange(int type);
    void widgetclean(int row, int mode, bool ismenu); //是否通过menu点击发出，用于区分是否下一个item直接进入hover状态
    void memorycleansignal();
private:
    MemoryListWidget *m_listwidget;
    QPoint m_mousepoint;
    IconButton *m_clearbutton;
    int m_type;
    bool m_isempty;
    Evaluator *m_evaluator;
    Quantity m_ans;
    bool m_ansVaild;
    QList<Quantity> m_list; //ans的list
    int m_themetype = 0;
    int m_calculatormode;//0-标准下拉 1-科学右侧
    int m_line = 1; //item数字行数
    MemoryItemDelegate *m_memoryDelegate;
    int m_itemwidth = 0;
    int m_precision = -1;//计算精度

private:
    QString formatExpression(const QString &text);
    QString setitemwordwrap(const QString &text, int row = 0);
//    void nothinginmemory(); //内存列表为空时插入一个item
    void emptymemoryfontcolor();
public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void setThemeType(int type);
    void expressionempty(bool b);
    void widgetcleanslot(int row, int mode, bool ismenu);
};


#endif // MEMORYWIDGET_H
