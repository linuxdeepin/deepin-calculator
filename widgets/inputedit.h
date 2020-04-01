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

#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <DLineEdit>
#include <DPalette>
#include "../math/quantity.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

struct SSelection {
    QString oldText;
    QString selected;
    QString clearText;
    bool isChanged;
    int curpos = 0;  // fix for bug-14249
    SSelection() { isChanged = false; }
};

class InputEdit : public QLineEdit
{
    Q_OBJECT

public:
    InputEdit(QWidget *parent = nullptr);
    ~InputEdit();

    QString expressionText();
    void setAnswer(const QString &str, const Quantity &ans);
    void clear();
    SSelection getSelection() { return m_selected; };
    void setSelection(SSelection select) { m_selected = select; };
    void setPercentAnswer(const QString &str1, const QString &str2, const Quantity &ans,
                          const int &Pos);

public slots:
    void setUndoAction(bool state);
    void setRedoAction(bool state);
    QString symbolFaultTolerance(const QString &text);

Q_SIGNALS:
    void keyPress(QKeyEvent *);
    void equal();
    void undo();
    void redo();
    void paste();
    void copy();
    void cut();
    void selectAllText();
    void deleteText();
    void setResult();

protected:
    void keyPressEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void initAction();
    void updateAction();
    bool isSymbolCategoryChanged(int pos1, int pos2);
    int findWordBeginPosition(int pos);
    int findWordEndPosition(int pos);
    void autoZoomFontSize();
    void handleTextChanged(const QString &text);
    void handleCursorPositionChanged(int oldPos, int newPos);
    void BracketCompletion(QKeyEvent *e);
    QString pointFaultTolerance(const QString &text);

    bool isSymbol(const QString &text);
    bool eventFilter(QObject *watched, QEvent *event);
    void multipleArithmetic(QString &text);
    void showTextEditMenu(QPoint p);
    void selectionChangedSlot();
    void pressSlot();

private:
    Quantity m_ans;
    int m_ansStartPos;
    int m_ansLength;
    bool m_ansVaild;
    bool m_currentInAns;
    bool m_currentOnAnsLeft;
    QString m_oldText;
    SSelection m_selected;

    QAction *m_undo;
    QAction *m_redo;
    QAction *m_cut;
    QAction *m_copy;
    QAction *m_paste;
    QAction *m_delete;
    QAction *m_select;
};

#endif
