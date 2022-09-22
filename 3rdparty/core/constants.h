// Copyright (C) 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008-2009, 2016 @heldercorreia
// Copyright (C) 2009 Andreas Scherer <andreas_coder@freenet.de>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include <QObject>
#include <QStringList>

#include <memory>

struct Constant {
    QString category;
    QString name;
    QString unit;
    QString value;
};

class Constants : public QObject {
    Q_OBJECT

public:
    ~Constants(); //  For unique_ptr, define after Private is complete.
    static Constants* instance();
    const QStringList& categories() const;
    const QList<Constant>& list() const;

public slots:
    void retranslateText();

private:
    struct Private;
    std::unique_ptr<Private> d;

    Constants();
    Q_DISABLE_COPY(Constants)
};

#endif*/

