#include "actionconfigurepage.h"

#include <QMWidgets/qmdecoratorv2.h>

#include <QLabel>

namespace Core::Internal {

    ActionConfigurePage::ActionConfigurePage(QObject *parent)
        : ISettingPage(QStringLiteral("core.ActionConfigure"), parent) {
        m_widget = nullptr;
        setTitle([]() { return tr("Action Layouts"); });
        setDescription([]() { return tr("Action Layouts"); });
    }

    ActionConfigurePage::~ActionConfigurePage() {
    }

    QString ActionConfigurePage::sortKeyword() const {
        return QStringLiteral("Action Layouts");
    }

    bool ActionConfigurePage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *ActionConfigurePage::widget() {
        if (!m_widget) {
            auto label = new QLabel();
            label->setText("Action Layouts");
            m_widget = label;
        }
        return m_widget;
    }

    bool ActionConfigurePage::accept() {
        return true;
    }

    void ActionConfigurePage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}
