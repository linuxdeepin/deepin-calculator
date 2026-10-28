// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logger.h"
#include "ddlog.h"
#include <QLoggingCategory>
#include <QObject>

#ifdef DTKCORE_CLASS_DConfigFile
#include <DConfig>
#endif
DCORE_USE_NAMESPACE

MLogger::MLogger(QObject *parent)
    : QObject(parent), m_rules("")
#ifdef DTKCORE_CLASS_DConfigFile
    , m_config(nullptr)
#endif
{
    QByteArray logRules = qgetenv("QT_LOGGING_RULES");
    // qunsetenv 之前一定不要有任何日志打印，否则取消环境变量设置不会生效
    qunsetenv("QT_LOGGING_RULES");

    // set env
    m_rules = logRules;

#ifdef DTKCORE_CLASS_DConfigFile
    // set dconfig
    m_config = DConfig::create("org.deepin.calculator", "org.deepin.calculator");
    logRules = m_config->value("log_rules").toByteArray();
    appendRules(logRules);
    setRules(m_rules);

    // watch dconfig
    // LCOV_EXCL_START
    connect(m_config, &DConfig::valueChanged, this, [this](const QString &key) {
        if (key == "log_rules") {
            setRules(m_config->value(key).toByteArray());
        }
    });
    // LCOV_EXCL_STOP
#else
    setRules(m_rules);
#endif
}

MLogger::~MLogger()
{
#ifdef DTKCORE_CLASS_DConfigFile
    if (m_config) {
        m_config->deleteLater();
    }
#endif
}

void MLogger::setRules(const QString &rules)
{
    auto tmpRules = rules;
    m_rules = tmpRules.replace(";", "\n");
    QLoggingCategory::setFilterRules(m_rules);
}

void MLogger::appendRules(const QString &rules)
{
    QString tmpRules = rules;
    tmpRules = tmpRules.replace(";", "\n");
    auto tmplist = tmpRules.split('\n');
    for (int i = 0; i < tmplist.count(); i++)
        if (m_rules.contains(tmplist.at(i))) {
            tmplist.removeAt(i);
            i--;
        }
    if (tmplist.isEmpty())
        return;
    m_rules.isEmpty() ? m_rules = tmplist.join("\n")
                      : m_rules += "\n" + tmplist.join("\n");
}
