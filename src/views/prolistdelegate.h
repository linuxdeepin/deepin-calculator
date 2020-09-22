#ifndef PROLISTDELEGATE_H
#define PROLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

class ProListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ProListDelegate(QObject *parent = nullptr);
    ~ProListDelegate();

public slots:
    void setThemeType(int type);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    void obtainingHistorical(const QModelIndex &index);

private:
    int m_themeType;
};

#endif // PROLISTDELEGATE_H
