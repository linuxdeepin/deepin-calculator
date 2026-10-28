// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DDLOG_H
#define DDLOG_H

#include <QDebug>
#include <QLoggingCategory>

// 全项目统一的日志分类，便于 DConfig(deepin-debug-config) 按应用统一过滤。
// 分类名必须与 desktop 文件名(deepin-calculator)对应的 org.deepin. 前缀保持一致。
inline Q_LOGGING_CATEGORY(appLog, "org.deepin.calculator")

#endif   // DDLOG_H
