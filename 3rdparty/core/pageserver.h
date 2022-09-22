// Copyright (C) 2014-2016 @heldercorreia
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_PAGESERVER_H
#define CORE_PAGESERVER_H

#include <QHash>
#include <QObject>
#include <QString>

class PageServer : public QObject {
    Q_OBJECT
public:
    explicit PageServer(QObject* parent = 0) : QObject(parent) { }
    QString getPageContent(const QString& id);
    QString getCurrentPageContent();

protected:
    typedef QString (*PageMaker)();
    void addPage(const QString& id, PageMaker maker) { m_toc[id] = maker; }
    virtual void createPages() = 0;

private:
    Q_DISABLE_COPY(PageServer)
    QHash<QString, PageMaker> m_toc;
    QString m_currentPageID;
};

#endif // CORE_PAGESERVER_H
