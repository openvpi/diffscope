#include "actionlayoutspage.h"

#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmappextension.h>

#include <QLabel>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "icore.h"

// Setting Page Roles
// 1. Action Layouts:
//      - Object:   ActionManager::instance()->actionDomain()
//      - Property: layouts
//      - Type:     QByteArray
// 2. Action Icons
//      - Object:   ActionManager::instance()->actionDomain()
//      - Property: overriddenIcons
//      - Type:     QJsonObject

namespace Core::Internal {

    ActionLayoutsPage::ActionLayoutsPage(QObject *parent)
        : ISettingPage(QStringLiteral("core.ActionLayout"), parent) {
        m_widget = nullptr;
        setTitle([]() { return tr("Action Layouts"); });
        setDescription([]() { return tr("Action Layouts"); });
    }

    ActionLayoutsPage::~ActionLayoutsPage() {
    }

    QString ActionLayoutsPage::sortKeyword() const {
        return QStringLiteral("Action Layouts");
    }

    bool ActionLayoutsPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *ActionLayoutsPage::widget() {
        if (!m_widget) {
            auto label = new QLabel();
            label->setText("Action Layouts");
            m_widget = label;
        }
        return m_widget;
    }

    bool ActionLayoutsPage::accept() {
        return true;
    }

    void ActionLayoutsPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}
