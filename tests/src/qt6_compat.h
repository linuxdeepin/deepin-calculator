// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef QT6_COMPAT_H
#define QT6_COMPAT_H

// Qt6 compatibility utilities for unit tests

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QEnterEvent>
#include <QEvent>
#include <QPointF>

/**
 * @brief Create a QEnterEvent compatible with Qt6
 *
 * In Qt5, enter events used QEvent::Type::Enter with QWidget::enterEvent(QEvent*).
 * In Qt6, enter events use QEnterEvent with QWidget::enterEvent(QEnterEvent*).
 */
inline QEnterEvent *createEnterEvent(const QPointF &localPos = QPointF(),
                                     const QPointF &globalPos = QPointF())
{
    return new QEnterEvent(localPos, localPos, globalPos);
}

/**
 * @brief Create a QEvent for Leave type (same in Qt5 and Qt6)
 */
inline QEvent *createLeaveEvent()
{
    return new QEvent(QEvent::Type::Leave);
}

/**
 * @brief Compatibility macro for Dtk6 theme switching
 *
 * In Dtk6, DGuiApplicationHelper::setThemeType() was replaced by
 * setPaletteType(ColorType) (LightType/DarkType). Probe-verified on DTK6:
 * setPaletteType drives the application palette the way Dtk5's
 * setThemeType did, so tests can still force dark/light themes.
 */
#include <DGuiApplicationHelper>
DGUI_USE_NAMESPACE
#define DTK_SET_THEME_TYPE(type) DGuiApplicationHelper::instance()->setPaletteType(type)

#else

// Qt5 compat - these are the original patterns
#include <QEnterEvent>
#include <QEvent>
#include <QPointF>

inline QEnterEvent *createEnterEvent(const QPointF &localPos = QPointF(),
                                     const QPointF &globalPos = QPointF())
{
    return new QEnterEvent(localPos, localPos, globalPos);
}

inline QEvent *createLeaveEvent()
{
    return new QEvent(QEvent::Type::Leave);
}

// In Qt5/Dtk5, setThemeType is available
#include <DGuiApplicationHelper>
#define DTK_SET_THEME_TYPE(type) DGuiApplicationHelper::instance()->setThemeType(type)

#endif

#endif // QT6_COMPAT_H
