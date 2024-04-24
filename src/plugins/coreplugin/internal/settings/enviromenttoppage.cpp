#include "enviromenttoppage.h"

#include <QMWidgets/qmdecoratorv2.h>

#include <QLabel>

namespace Core::Internal {

    EnvironmentTopPage::EnvironmentTopPage(QObject *parent)
        : ISettingPage(QStringLiteral("core.EnvironmentTop"), parent) {
        setTitle([]() { return tr("Environment"); });
        setDescription([]() { return tr("Application general environment and behaviors."); });
    }

    EnvironmentTopPage::~EnvironmentTopPage() {
    }

    QString EnvironmentTopPage::sortKeyword() const {
        return QStringLiteral("Environment");
    }

    bool EnvironmentTopPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *EnvironmentTopPage::widget() {
        return nullptr;
    }

    bool EnvironmentTopPage::accept() {
        return true;
    }

    void EnvironmentTopPage::finish() {
    }

}