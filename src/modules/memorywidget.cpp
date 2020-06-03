#include "memorywidget.h"

#include <QDebug>
#include <QFont>
#include <QModelIndex>
#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QEvent>
#include <QPen>
#include <QContextMenuEvent>
#include <QMenu>
#include <QClipboard>
#include <DGuiApplicationHelper>
#include "../utils.h"

MemoryWidget::MemoryWidget(int mode, QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new MemoryListWidget(this))
    , m_clearbutton(new IconButton(this, 1))
    , m_isempty(true)
    , memoryDelegate(new MemoryItemDelegate(this))
{
    calculatormode = mode;
    m_evaluator = Evaluator::instance();
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *layH = new QHBoxLayout();

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);

//    DPalette pal = m_listwidget->palette();
//    pal.setColor(DPalette::Light, QColor(248, 248, 248));
//    m_listwidget->setPalette(pal);

    m_listwidget->setFrameShape(QFrame::NoFrame);
    mode == 0 ? m_listwidget->setFixedHeight(260) : m_listwidget->setFixedHeight(500);
    itemwidth = (mode == 0) ? 344 : 370;
    precision = (mode == 0) ? 15 : 31;
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listwidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listwidget->setAutoScroll(false);
    m_listwidget->setSelectionRectVisible(false);
    m_listwidget->setFocusPolicy(Qt::NoFocus);
    m_listwidget->setUniformItemSizes(false);
    m_listwidget->setItemDelegate(memoryDelegate);
    nothinginmemory();
    lay->addStretch();
    layH->addStretch();

    layH->addWidget(m_clearbutton);
    if (mode == 1)
        m_clearbutton->hide();
    connect(m_clearbutton, &DPushButton::clicked, this, [ = ]() {
        nothinginmemory();
    });
    lay->addLayout(layH);
    this->setLayout(lay);
    connect(m_listwidget, &MemoryListWidget::itemselected, this, [ = ](int row) {
        if (!m_isempty) {
            QPair<QString, Quantity> p;
            MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
            //        p.first = m_listwidget->item(row)->data(Qt::DisplayRole).toString();
            p.first = w1->textLabel();
            p.second = list.at(row);
            if (m_listwidget->item(row)->flags() != Qt::NoItemFlags)
                emit itemclick(p);
        }
    });
}

void MemoryWidget::generateData(Quantity answer)
{
    //500 memory number limit
    if (list.count() == 500) {
        list.pop_back();
        m_listwidget->takeItem(499);
    }
    if (m_isempty == true) {
        m_listwidget->clear();
    }
    m_isempty = false;
    emit mListAvailable();
    QListWidgetItem *item1 = new QListWidgetItem();
    item1->setFlags(Qt::ItemIsSelectable);
//    item1->setTextAlignment(Qt::AlignRight | Qt::AlignTop);
    QFont font;
    font.setPixelSize(30);
    item1->setFont(font);
    item1->setSizeHint(QSize(itemwidth, 40 + 45 * line));
    MemoryItemWidget *widget = new MemoryItemWidget(this);
    widget->setFixedSize(QSize(itemwidth, 40 + 45 * line));

    m_listwidget->insertItem(0, item1);
    m_listwidget->setItemWidget(item1, widget);
    if (answer == Quantity(0)) {
//        item1->setData(Qt::DisplayRole, "0");
        widget->setTextLabel("0");
    } else {
        const QString result = DMath::format(answer, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
//        item1->setData(Qt::DisplayRole, formatResult);
        widget->setTextLabel(formatResult);
    }
    widget->setLineHight(line);
    list.insert(0, answer); //对于新增数据，同步在list中加入对应的Quantity
    connect(widget, &MemoryItemWidget::plusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
//        widget->setFocus();
        emit MemoryWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::minusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
//        widget->setFocus();
        emit MemoryWidget::widgetminus(row);
    });
    connect(widget, &MemoryItemWidget::cleanbtnclicked, this, [ = ]() {
//        widget->setFocus();
        emit widgetclean(m_listwidget->row(item1), calculatormode);
        list.removeAt(m_listwidget->row(item1));
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            nothinginmemory();
        }
    });
    widget->themetypechanged(m_themetype);
    connect(this, &MemoryWidget::themechange, widget, &MemoryItemWidget::themetypechanged);
    connect(widget, &MemoryItemWidget::itemchanged, this, [ = ](int type) {
        widget->update();
    });
    connect(widget, &MemoryItemWidget::menuclean, this, [ = ]() {
        emit widgetclean(m_listwidget->row(item1), calculatormode);
        list.removeAt(m_listwidget->row(item1));
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            nothinginmemory();
        }
    });
    connect(widget, &MemoryItemWidget::menucopy, this, [ = ]() {
        QClipboard *clipboard = QApplication::clipboard();
        QString originalText = clipboard->text();
//        clipboard->setText(item1->data(Qt::EditRole).toString().remove("\n"));
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(item1));
        clipboard->setText(w1->textLabel().remove("\n"));
    });
    connect(widget, &MemoryItemWidget::menuplus, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::menuminus, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetminus(row);
    });
}

void MemoryWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (m_themetype == 1) {
        QPen pen(QBrush(QColor("#F8F8F8")), 0);
        painter.setPen(pen);
    } else {
        QPen pen(QBrush(QColor("#252525")), 0);
        painter.setPen(pen);
    }
//    painter.setBrush(QBrush(QColor("#F8F8F8")));
    QRect rect(this->rect().left(), this->rect().top(), this->rect().width(), this->rect().bottom() + 30);
    painter.drawRect(rect);
    QWidget::paintEvent(event);
}

void MemoryWidget::mousePressEvent(QMouseEvent *event)
{
    m_type = -1;
    QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
    m_mousepoint = pEvent->pos();

    QRect rect(this->frameGeometry());
    if (rect.contains(m_mousepoint) == true)
        emit insidewidget();
    QWidget::mousePressEvent(event);
}

void MemoryWidget::memoryplus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
//        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "+(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
//        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        w1->setTextLabel(formatResult);
        list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(answer);
    }
}

void MemoryWidget::memoryminus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
//        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
//        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        w1->setTextLabel(formatResult);
        list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(Quantity(0));
        QString exp = QString("0-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
//        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        w1->setTextLabel(formatResult);
        list.replace(0, ans);
    }
}

void MemoryWidget::memoryclean()
{
    nothinginmemory();
}

void MemoryWidget::emptymemoryfontcolor()
{
    if (m_themetype == 1) {
        m_listwidget->item(0)->setTextColor(QColor(85, 85, 85, 0.4 * 255));
    } else {
        m_listwidget->item(0)->setTextColor(QColor(192, 198, 212, 0.4 * 255));
    }
}

QPair<QString, Quantity> MemoryWidget::getfirstnumber()
{
    QPair<QString, Quantity> p1;
    if (m_isempty == false) {
//        QString str = m_listwidget->item(0)->data(Qt::EditRole).toString();
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        QString str = w1->textLabel();
        p1.first = str.remove("\n");
        p1.second = list.at(0);
        return p1;
    } else {
        p1.first = QString();
        p1.second = Quantity(0);
        return p1;
    }
}

void MemoryWidget::widgetplusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0)) {
//        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    } else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "+(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult, row);
//        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->setTextLabel(formatResult);
        list.replace(row, ans);
    }
}

void MemoryWidget::widgetminusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0)) {
        //        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    } else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult, row);
//        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->setTextLabel(formatResult);
        list.replace(row, ans);
    }
}

void MemoryWidget::expressionempty(bool b)
{
    if (!m_isempty) {
        for (int i = 0; i < m_listwidget->count(); i++) {
            static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(i)))->isexpressionempty(b);
        }
    }
}

void MemoryWidget::widgetcleanslot(int row, int mode)
{
    if (calculatormode != mode) {
        list.removeAt(row);
        delete m_listwidget->takeItem(row);
        if (m_listwidget->count() == 0) {
            nothinginmemory();
        }
    }
}

QString MemoryWidget::formatExpression(const QString &text)
{
    return QString(text)
           .replace(QString::fromUtf8("＋"), "+")
           .replace(QString::fromUtf8("－"), "-")
           .replace(QString::fromUtf8("×"), "*")
           .replace(QString::fromUtf8("÷"), "/")
           .replace(QString::fromUtf8(","), "");
}

QString MemoryWidget::setitemwordwrap(const QString &text, int row)
{
    QString result = text;
    result.replace('-', "－").replace('+', "＋");
    int index = result.indexOf("e");
    line = 1;
    if (calculatormode == 0) {
        if (index > 0 && result.left(index).length() > 13) {
            result.insert(index, "\n");
            line = 2;
        } else if (index <= 0 && result.length() > 21) {
            result.insert(20, "\n");
            line = 2;
        }
        if (m_clearbutton->isHidden() == true) {
            m_clearbutton->show();
            m_clearbutton->settooltip(true);
        }
    } else {
        if (index > 15) {
            result.insert(index, "\n");
            line = 2;
            if (index > 20) {
                result.remove("\n");
                result.insert(20, "\n");
                line = 2;
//                if (index > 33 && result.right(result.length() - index - 1).length() > 3) {
//                    result.insert(index + 1, "\n");
//                    line = 3;
//                }
            }
        } else if (index <= 0 && result.length() > 21) {
            result.insert(20, "\n");
            line = 2;
        }
    }
    if (m_listwidget->item(row)) {
        m_listwidget->item(row)->setSizeHint(QSize(itemwidth, 40 + 45 * line));
        m_listwidget->itemWidget(m_listwidget->item(row))->setFixedSize(QSize(itemwidth, 40 + 45 * line));
    }
    return result;
}

void MemoryWidget::nothinginmemory()
{
    m_listwidget->clear();
    list.clear();
    m_listwidget->addItem(tr("Nothing saved in memory"));
    QFont m_clearbuttonfont;
    m_clearbuttonfont.setPixelSize(16);
    m_listwidget->item(0)->setFont(m_clearbuttonfont);
    m_listwidget->item(0)->setSizeHint(QSize(itemwidth, m_listwidget->frameRect().height()));
    m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
    m_listwidget->item(0)->setTextAlignment(Qt::AlignCenter | Qt::AlignTop);
    emptymemoryfontcolor();
    m_isempty = true;
    m_clearbutton->hide();
    emit mListUnavailable();
}

void MemoryWidget::setThemeType(int type)
{
    m_listwidget->update();
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_themetype = typeIn;
    emit themechange(m_themetype);
    QString path;
    if (m_themetype == 1) {
        path = QString(":/assets/images/%1/").arg("light");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
        if (m_isempty) {
            emptymemoryfontcolor();
        }
    } else {
        path = QString(":/assets/images/%1/").arg("dark");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
        if (m_isempty) {
            emptymemoryfontcolor();
        }
    }
}

MemoryWidget::~MemoryWidget()
{

}
