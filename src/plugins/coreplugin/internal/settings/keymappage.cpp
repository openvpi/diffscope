#include "keymappage.h"

#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmappextension.h>

#include <QLabel>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <QLabel>

#include "icore.h"

// Setting Page Roles
// 1. Action Shortcuts
//      - Object:   ActionManager::instance()->actionDomain()
//      - Property: overriddenShortcuts
//      - Type:     QJsonObject
// 2. Current Action Family
//      - Object:   ActionManager::instance()
//      - Property: currentShortcutFamily
//      - Type:     QString

namespace Core::Internal {

    KeymapPage::KeymapPage(QObject *parent) : ISettingPage(QStringLiteral("core.Keymap"), parent) {
        m_widget = nullptr;
        setTitle([]() { return tr("Keymap"); });
        setDescription([]() { return tr("Keyboard shortcuts."); });
    }

    KeymapPage::~KeymapPage() = default;

    QString KeymapPage::sortKeyword() const {
        return QStringLiteral("Keymap");
    }

    bool KeymapPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }

    QWidget *KeymapPage::widget() {
        if (!m_widget) {
            auto label = new QLabel();
            label->setText("Keymap");
            m_widget = label;
        }
        return m_widget;
    }

    bool KeymapPage::accept() {

        return true;
    }

    void KeymapPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}
