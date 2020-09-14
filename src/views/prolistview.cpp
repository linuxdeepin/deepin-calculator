#include "prolistview.h"

ProListView::ProListView(DWidget *parent)
    : DListView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows); //选中一行
    setSelectionMode(QAbstractItemView::SingleSelection); //选中单个目标
    setFixedSize(427, 106);

    connect(this, &DListView::clicked, [ = ](const QModelIndex & index) {
        this->setCurrentIndex(index);
        emit obtainingHistorical(index);
    });
}

ProListView::~ProListView()
{

}

void ProListView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    //缺翻译
    QAction *copy = new QAction(tr("Copy"), menu);
    menu->addAction(copy);
    connect(copy, &QAction::triggered, this, [ = ]() {
        static_cast<ProListModel *>(model())->copyToClipboard(indexAt(event->pos()).row());
    });
    menu->exec(event->globalPos());
    delete menu;
}

void ProListView::showTextEditMenuByAltM(const QModelIndex &index)
{
    QMenu *menu = new QMenu(this);
    //缺翻译
    QAction *copy = new QAction(tr("Copy"), menu);
    menu->addAction(copy);
    connect(copy, &QAction::triggered, this, [ = ]() {
        static_cast<ProListModel *>(model())->copyToClipboard(index.row());
    });
    QPoint menupoint;
    menupoint.setX(mapToGlobal(visualRect(index).bottomRight()).x() - 10);
    menupoint.setY(mapToGlobal(visualRect(index).center()).y());
    menu->exec(menupoint);
    delete menu;
}

void ProListView::keyPressEvent(QKeyEvent *e)
{
    bool ispressalt = e->modifiers() == Qt::AltModifier;
    switch (e->key()) {
//    case Qt::Key_Up:
//        if (currentIndex().row() > 0) {
//            setCurrentIndex(this->model()->index(currentIndex().row() - 1, 0));
//            static_cast<SimpleListDelegate *>(itemDelegate(currentIndex()))->currentfocusindex(currentIndex());
//            m_currentindexrow = currentIndex().row();
//            scrollTo(this->model()->index(currentIndex().row(), 0));
//        }
//        break;
//    case Qt::Key_Down:
//        if (currentIndex().row() < this->count() - 1) {
//            setCurrentIndex(this->model()->index(currentIndex().row() + 1, 0));
//            static_cast<SimpleListDelegate *>(itemDelegate(currentIndex()))->currentfocusindex(currentIndex());
//            m_currentindexrow = currentIndex().row();
//            scrollTo(this->model()->index(currentIndex().row(), 0));
//        }
//        break;
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
