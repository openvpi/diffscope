#include "appearancetoppage.h"

#include <QMWidgets/qmdecoratorv2.h>

#include <QLabel>

namespace Core::Internal {

    AppearanceTopPage::AppearanceTopPage(QObject *parent)
        : ISettingPage("core.AppearanceTop", parent) {
        setTitle([]() { return tr("Appearance"); });
        setDescription([]() { return tr("Application appearance."); });
    }

    AppearanceTopPage::~AppearanceTopPage() {
    }

    QString AppearanceTopPage::sortKeyword() const {
        return QStringLiteral("Appearance");
    }

    bool AppearanceTopPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *AppearanceTopPage::widget() {
        return nullptr;
    }

    bool AppearanceTopPage::accept() {
        return true;
    }

    void AppearanceTopPage::finish() {
    }

}