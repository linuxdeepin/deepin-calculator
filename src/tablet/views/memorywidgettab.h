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

#ifndef MEMORYWIDGETTAB_H
#define MEMORYWIDGETTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QList>
#include <QObject>
#include <DPushButton>

#include "core/evaluator.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/views/memoryitemwidgettab.h"
#include "views/memorylistwidget.h"
#include "views/memoryitemdelegate.h"

/**
 * @brief 标准模式内存界面
 */
class MemoryPublicTab;
class MemoryWidgetTab : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidgetTab(int mode = 0, QWidget *parent = nullptr);
    ~MemoryWidgetTab() Q_DECL_OVERRIDE;

    void generateData(const Quantity answer, bool ismax);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void memoryAnsChanged(int row, const Quantity answer);
    void memoryclean();//内存列表为空时插入一个item
    QPair<QString, Quantity> getfirstnumber();//MR
    MemoryListWidget *getMemoryWidget();

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void insidewidget();
    void itemclick(const QPair<QString, Quantity>);
    void themechange(int type);
    void widgetclean(int row, int mode, bool ismenu); //是否通过menu点击发出，用于区分是否下一个item直接进入hover状态
    void scimemtab();
    void basicPressEscape();

private:
    MemoryListWidget *m_listwidget;
    QPoint m_mousepoint;
    Evaluator *m_evaluator;
    Quantity m_ans;
    bool m_ansVaild;
    int m_themetype = 0;
    int m_calculatormode;//0-标准下拉 1-科学右侧
    int m_line = 1; //item数字行数
    MemoryItemDelegate *m_memoryDelegate;
    int m_itemwidth = 0;
    int m_precision = -1;//计算精度
    QLabel *m_label;//内存为空item
    int m_currentrow = 0;//listwidget当前行
    MemoryPublicTab *m_memorypublic = nullptr;
    int m_mode;

private:
    void initConnect();
    QString formatExpression(const QString &text);
    QString setitemwordwrap(const QString &text, int row = 0);
    void emptymemoryfontcolor();
public slots:
    void setThemeType(int type);
    void expressionempty(bool b);
    void widgetcleanslot(int row, int mode, bool ismenu);
    void resetWidgetSize(QSize size);
    void setMemoryPublic(MemoryPublicTab *pub);
};


#endif // MEMORYWIDGETTAB_H
