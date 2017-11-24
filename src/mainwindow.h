#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMenu>

#include "titlebar.h"
#include "expressionlist.h"
#include "textbutton.h"
#include "backbutton.h"
#include "dsettings.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(DMainWindow *parent=0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void initTheme();
    void initThemeAction();
    void switchTheme();
    void onNumberButtonClicked();
    void onSymbolButtonClicked();
    void clearButtonStateChanged(bool isAllClear);

private:
    QWidget *m_mainWidget;
    QMenu *m_menu;
    QAction *m_themeAction;
    DSettings *m_settings;
    QGridLayout *m_mainLayout;
    TitleBar *m_titleBar;
    ExpressionList *m_expList;
    TextButton *clearButton;
    BackButton *backButton;
    TextButton *modButton;
    TextButton *divButton;
    TextButton *num7Button;
    TextButton *num8Button;
    TextButton *num9Button;
    TextButton *multButton;
    TextButton *num4Button;
    TextButton *num5Button;
    TextButton *num6Button;
    TextButton *minButton;
    TextButton *num1Button;
    TextButton *num2Button;
    TextButton *num3Button;
    TextButton *plusButton;
    TextButton *zeroButton;
    TextButton *pointButton;
    TextButton *bracketsButton;
    TextButton *equalButton;
    QString m_titlebarColor;
    QString m_separatorColor;
    QColor m_backgroundColor;
};

#endif
