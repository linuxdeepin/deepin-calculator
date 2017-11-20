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
    void changeTheme(QString theme);
    void onNumberButtonClicked();
    void onSymbolButtonClicked();
    void clearButtonStateChanged(bool isAllClear);

private:
    QWidget *mainWidget;
    QMenu *menu;
    QAction *themeAction;
    DSettings *settings;
    QGridLayout *mainLayout;
    TitleBar *titleBar;
    ExpressionList *expList;
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
    QString titlebarColor;
    QString separatorColor;
    QColor backgroundColor;
};

#endif
