// Copyright (C) 2014 @heldercorreia
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pageserver.h"

QString PageServer::getPageContent(const QString &id)
{
    PageMaker maker = m_toc.value(id);
    if (!maker)
        return QString();
    m_currentPageID = id;
    return maker();
}

QString PageServer::getCurrentPageContent()
{
    if (m_currentPageID.isNull())
        return QString();
    return getPageContent(m_currentPageID);
}
