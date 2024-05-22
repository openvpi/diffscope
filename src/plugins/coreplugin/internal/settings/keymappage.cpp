#include "keymappage.h"

#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmappextension.h>

#include <QLabel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QApplication>
#include <QMessageBox>
#include <QKeySequenceEdit>
#include <QInputDialog>

#include "icore.h"

// Setting Page Roles
// 1. Action Shortcuts
//      - Object:   ActionManager::instance()->actionDomain()
//      - Property: overriddenShortcuts
//      - Type:     QJsonObject
// 2. Current Action Family
//      - Object:   ActionManager::instance()
//      - Property: currentShortcutsFamily
//      - Type:     QString

namespace Core::Internal {

    class KeymapPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit KeymapPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;

            auto presetLayout = new QFormLayout;
            auto presetRowLayout = new QHBoxLayout;
            m_presetComboBox = new QComboBox;
            presetRowLayout->addWidget(m_presetComboBox, 1);
            auto presetSaveAsButton = new QPushButton(tr("Save &As"));
            presetRowLayout->addWidget(presetSaveAsButton);
            auto presetDeleteButton = new QPushButton(tr("&Delete"));
            presetRowLayout->addWidget(presetDeleteButton);
            auto presetRenameButton = new QPushButton(tr("&Rename"));
            presetRowLayout->addWidget(presetRenameButton);
            auto presetLabel = new QLabel(tr("&Preset"));
            presetLabel->setBuddy(m_presetComboBox);
            presetLayout->addRow(presetLabel, presetRowLayout);
            mainLayout->addLayout(presetLayout);

            auto actionsGroupBox = new QGroupBox(tr("Actions"));
            auto actionsLayout = new QVBoxLayout;
            auto actionsFilterLayout = new QHBoxLayout;
            auto expandAllButton = new QPushButton(tr("&Expand All"));
            actionsFilterLayout->addWidget(expandAllButton);
            auto collapseAllButton = new QPushButton(tr("&Collapse All"));
            actionsFilterLayout->addWidget(collapseAllButton);
            auto filterLineEdit = new QLineEdit;
            filterLineEdit->setPlaceholderText(tr("Filter"));
            actionsFilterLayout->addWidget(filterLineEdit, 1);
            auto findShortcutButton = new QPushButton(tr("&Find Shortcut"));
            actionsFilterLayout->addWidget(findShortcutButton);
            actionsLayout->addLayout(actionsFilterLayout);
            auto actionsTreeWidget = new QTreeWidget;
            actionsTreeWidget->setHeaderLabels({tr("Name"), tr("ID"), tr("Shortcut")});
            actionsLayout->addWidget(actionsTreeWidget);
            actionsGroupBox->setLayout(actionsLayout);
            mainLayout->addWidget(actionsGroupBox);

            setLayout(mainLayout);

            {
                int currentPresetIndex = -1;
                for (const auto &id : ICore::instance()->actionManager()->systemShortcutsFamilies()) {
                    m_presetComboBox->addItem(id, true);
                    if (currentPresetIndex == -1) {
                        auto [shortcutsFamilyId, isSystemShortcutsFamily] = ICore::instance()->actionManager()->currentShortcutsFamily();
                        if (isSystemShortcutsFamily && shortcutsFamilyId == id)
                            currentPresetIndex = m_presetComboBox->count() - 1;
                    }
                }
                for (const auto &id : ICore::instance()->actionManager()->userShortcutFamilies()) {
                    m_presetComboBox->addItem(id, false);
                    if (currentPresetIndex == -1) {
                        auto [shortcutsFamilyId, isSystemShortcutsFamily] = ICore::instance()->actionManager()->currentShortcutsFamily();
                        if (!isSystemShortcutsFamily && shortcutsFamilyId == id)
                            currentPresetIndex = m_presetComboBox->count() - 1;
                    }
                }
                if (currentPresetIndex != -1)
                    m_presetComboBox->setCurrentIndex(currentPresetIndex);
                presetDeleteButton->setDisabled(m_presetComboBox->currentData().toBool());
                presetRenameButton->setDisabled(m_presetComboBox->currentData().toBool());
            }

            for (const auto &catalog : ICore::instance()->actionManager()->domain()->catalog().children()) {
                auto item = new QTreeWidgetItem;
                traverseCatalog(item, catalog);
                traverseShortcuts(item);
                actionsTreeWidget->addTopLevelItem(item);
            }
            actionsTreeWidget->expandAll();
            actionsTreeWidget->resizeColumnToContents(0);
            actionsTreeWidget->resizeColumnToContents(1);
            actionsTreeWidget->collapseAll();
            actionsTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(m_presetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
                m_shortcutInfo.clear();
                for (int i = 0; i < actionsTreeWidget->topLevelItemCount(); i++) {
                    auto item = actionsTreeWidget->topLevelItem(i);
                    traverseShortcuts(item);
                }
                presetDeleteButton->setDisabled(m_presetComboBox->currentData().toBool());
                presetRenameButton->setDisabled(m_presetComboBox->currentData().toBool());
            });

            connect(presetSaveAsButton, &QAbstractButton::clicked, this, [=] {
                auto name = promptPresetName(tr("Save \"%1\" As").arg(m_presetComboBox->currentText()));
                if (name.isEmpty())
                    return;
                auto actionMgr = ICore::instance()->actionManager();
                auto shortcutsFamily = m_presetComboBox->currentData().toBool() ? actionMgr->systemShortcutsFamily(m_presetComboBox->currentText()) : actionMgr->userShortcutsFamily(m_presetComboBox->currentText());
                auto duplicatedShortcutFamily = duplicateShortcutsFamily(
                    shortcutsFamily,
                    m_presetComboBox->currentData().toBool() ? decltype(m_userShortcutFamilyAddition)::mapped_type() : m_userShortcutFamilyAddition.value(m_presetComboBox->currentText()),
                    m_presetComboBox->currentData().toBool() ? m_systemShortcutFamilyModification.value(m_presetComboBox->currentText()) : m_userShortcutFamilyModification.value(m_presetComboBox->currentText()));
                m_userShortcutFamilyModification.remove(name);
                m_userShortcutFamilyRemoval.remove(name);
                m_userShortcutFamilyAddition.insert(name, duplicatedShortcutFamily);
                for (int i = 0; i <= m_presetComboBox->count(); i++) {
                    if (i == m_presetComboBox->count()) {
                        m_presetComboBox->addItem(name, false);
                        m_presetComboBox->setCurrentIndex(m_presetComboBox->count() - 1);
                    }
                    if (m_presetComboBox->itemText(i) == name && !m_presetComboBox->itemData(i).toBool()) {
                        m_presetComboBox->setCurrentIndex(i);
                        break;
                    }
                }
            });

            connect(presetDeleteButton, &QAbstractButton::clicked, this, [=] {
                auto name = m_presetComboBox->currentText();
                m_userShortcutFamilyModification.remove(name);
                m_userShortcutFamilyAddition.remove(name);
                m_userShortcutFamilyRemoval.insert(name);
                m_presetComboBox->removeItem(m_presetComboBox->currentIndex());
            });

            connect(presetRenameButton, &QAbstractButton::clicked, this, [=] {
                auto oldName = m_presetComboBox->currentText();
                auto name = promptPresetName(tr("Rename \"%1\" to").arg(oldName));
                if (name.isEmpty())
                    return;
                if (name == oldName)
                    return;

                m_userShortcutFamilyModification.remove(oldName);
                m_userShortcutFamilyAddition.remove(oldName);
                m_userShortcutFamilyRemoval.insert(oldName);

                auto actionMgr = ICore::instance()->actionManager();
                auto shortcutsFamily = m_presetComboBox->currentData().toBool() ? actionMgr->systemShortcutsFamily(m_presetComboBox->currentText()) : actionMgr->userShortcutsFamily(m_presetComboBox->currentText());
                auto duplicatedShortcutFamily = duplicateShortcutsFamily(
                    shortcutsFamily,
                    m_presetComboBox->currentData().toBool() ? decltype(m_userShortcutFamilyAddition)::mapped_type() : m_userShortcutFamilyAddition.value(m_presetComboBox->currentText()),
                    m_presetComboBox->currentData().toBool() ? m_systemShortcutFamilyModification.value(m_presetComboBox->currentText()) : m_userShortcutFamilyModification.value(m_presetComboBox->currentText()));
                m_userShortcutFamilyModification.remove(name);
                m_userShortcutFamilyRemoval.remove(name);
                m_userShortcutFamilyAddition.insert(name, duplicatedShortcutFamily);

                m_presetComboBox->setItemText(m_presetComboBox->currentIndex(), name);
            });

            connect(expandAllButton, &QAbstractButton::clicked, actionsTreeWidget, &QTreeWidget::expandAll);
            connect(collapseAllButton, &QAbstractButton::clicked, actionsTreeWidget, &QTreeWidget::collapseAll);

            connect(filterLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
                for (int i = 0; i < actionsTreeWidget->topLevelItemCount(); i++) {
                    auto item = actionsTreeWidget->topLevelItem(i);
                    traverseFilter(item, text);
                }
            });

            connect(findShortcutButton, &QAbstractButton::clicked, this, [=] {
                QDialog dlg(this);
                dlg.setWindowFlags(Qt::Popup);
                auto mainLayout = new QVBoxLayout;
                auto keySequenceEdit = new QKeySequenceEdit;
                mainLayout->addWidget(keySequenceEdit);
                dlg.setLayout(mainLayout);
                connect(keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, [=](const QKeySequence &shortcut) {
                    if (m_shortcutInfo.contains(shortcut.toString())) {
                        auto item = m_shortcutInfo.value(shortcut.toString());
                        actionsTreeWidget->setCurrentItem(item);
                    }
                });
                keySequenceEdit->setFocus();
                dlg.move(QCursor::pos());
                dlg.exec();
            });

            connect(actionsTreeWidget, &QTreeWidget::itemActivated, this, &KeymapPageWidget::handleItemContextMenu);
            connect(actionsTreeWidget, &QTreeWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
                auto *item = actionsTreeWidget->itemAt(pos);
                handleItemContextMenu(item);
            });

        }

        void applyModification() {
            auto actionMgr = ICore::instance()->actionManager();
            for (const auto &name : qAsConst(m_userShortcutFamilyRemoval)) {
                actionMgr->removeUserShortcutsFamily(name);
            }
            m_userShortcutFamilyRemoval.clear();
            for (const auto &name : m_userShortcutFamilyAddition.keys()) {
                auto addition = m_userShortcutFamilyAddition.value(name);
                actionMgr->addUserShortcutsFamily(name, ActionDomain::ShortcutsFamily(addition.cbegin(), addition.cend()));
            }
            m_userShortcutFamilyAddition.clear();
            for (const auto &name : m_userShortcutFamilyModification.keys()) {
                auto family = actionMgr->userShortcutsFamily(name);
                auto modification = m_userShortcutFamilyModification.value(name);
                family.insert(ActionDomain::ShortcutsFamily(modification.cbegin(), modification.cend()));
                actionMgr->removeUserShortcutsFamily(name);
                actionMgr->addUserShortcutsFamily(name, family);
            }
            m_userShortcutFamilyModification.clear();
            for (const auto &name : m_systemShortcutFamilyModification.keys()) {
                bool changed;
                auto presetCopy = duplicateShortcutsFamily(
                    actionMgr->systemShortcutsFamily(name),
                    {},
                    m_systemShortcutFamilyModification.value(name),
                    &changed);
                if (!changed)
                    continue;
                QString newName = name;
                do {
                    newName = tr("%1 (copy)").arg(newName);
                } while (actionMgr->userShortcutFamilies().contains(newName));
                actionMgr->addUserShortcutsFamily(newName, ActionDomain::ShortcutsFamily(presetCopy.cbegin(), presetCopy.cend()));
                m_presetComboBox->addItem(newName, false);
                if (m_presetComboBox->currentText() == name && m_presetComboBox->currentData().toBool())
                    m_presetComboBox->setCurrentIndex(m_presetComboBox->count() - 1);
            }
            m_systemShortcutFamilyModification.clear();
            actionMgr->setCurrentShortcutsFamily(m_presetComboBox->currentText(), m_presetComboBox->currentData().toBool());
        }

    private:
        enum ItemDataRole {
            ShortcutRole = Qt::UserRole,
        };

        QComboBox *m_presetComboBox;

        QHash<QString, QTreeWidgetItem *> m_shortcutInfo;
        QHash<QString, QHash<QString, QList<QKeySequence>>> m_systemShortcutFamilyModification;
        QHash<QString, QHash<QString, QList<QKeySequence>>> m_userShortcutFamilyModification;
        QHash<QString, QHash<QString, QList<QKeySequence>>> m_userShortcutFamilyAddition;
        QSet<QString> m_userShortcutFamilyRemoval;

        void traverseCatalog(QTreeWidgetItem *item, const ActionCatalog &catalog) const {
            auto domain = ICore::instance()->actionManager()->domain();
            item->setText(0, ActionObjectInfo::translatedCategory(catalog.name()));
            item->setIcon(0, domain->objectIcon(qIDec->theme(), catalog.id()));
            item->setText(1, catalog.id());
            for (const auto &childCatalog : catalog.children()) {
                auto childItem = new QTreeWidgetItem;
                traverseCatalog(childItem, childCatalog);
                item->addChild(childItem);
            }
        }

        void traverseShortcuts(QTreeWidgetItem *item) {
            auto actionMgr = ICore::instance()->actionManager();
            auto shortcutsFamily = m_presetComboBox->currentData().toBool() ? actionMgr->systemShortcutsFamily(m_presetComboBox->currentText()) : actionMgr->userShortcutsFamily(m_presetComboBox->currentText());
            auto domain = ICore::instance()->actionManager()->domain();
            auto id = item->text(1);
            for (int i = 0; i < item->childCount(); i++) {
                traverseShortcuts(item->child(i));
            }
            if (auto objectInfo = domain->objectInfo(id); objectInfo.isNull() || objectInfo.type() != ActionObjectInfo::Action)
                return;
            QStringList shortcutTexts;
            QList<QKeySequence> shortcuts;
            if (auto modification = (m_presetComboBox->currentData().toBool() ? m_systemShortcutFamilyModification : m_userShortcutFamilyModification).value(m_presetComboBox->currentText()); modification.contains(id)) {
                shortcuts = modification.value(id);
            } else if (!m_presetComboBox->currentData().toBool() && m_userShortcutFamilyAddition.contains(m_presetComboBox->currentText())) {
                shortcuts = m_userShortcutFamilyAddition.value(m_presetComboBox->currentText()).value(id, domain->objectInfo(id).shortcuts());
            } else if (shortcutsFamily.value(item->text(1))) {
                shortcuts = shortcutsFamily.value(id).value();
            } else {
                shortcuts = domain->objectInfo(id).shortcuts();
            }
            for (const auto &keySeq : qAsConst(shortcuts)) {
                shortcutTexts.append(QStringLiteral("[") + keySeq.toString(QKeySequence::NativeText) + QStringLiteral("]"));
                m_shortcutInfo.insert(keySeq.toString(), item);
            }

            item->setText(2, shortcutTexts.join(' '));
            item->setData(0, ShortcutRole, QVariant::fromValue(shortcuts));
        }

        bool traverseFilter(QTreeWidgetItem *item, const QString &text) const {
            bool childMatched = false;
            for (int i = 0; i < item->childCount(); i++) {
                auto childItem = item->child(i);
                childMatched = traverseFilter(childItem, text) || childMatched;
            }
            if (childMatched) {
                item->setHidden(false);
                if (!text.isEmpty())
                    item->setExpanded(true);
                return true;
            } else if (item->text(0).contains(text, Qt::CaseInsensitive) || item->text(1).contains(text, Qt::CaseInsensitive)) {
                item->setHidden(false);
                return true;
            } else {
                item->setHidden(true);
                return false;
            }
        }

        void handleItemContextMenu(QTreeWidgetItem *item) {
            if (!item || item->data(0, ShortcutRole).isNull())
                return;
            QMenu popupMenu(this);
            connect(popupMenu.addAction(tr("Add Shortcut")), &QAction::triggered, this, [=] {
                promptShortcut(item);
            });
            popupMenu.addSeparator();
            for (const auto &shortcut : item->data(0, ShortcutRole).value<QList<QKeySequence>>()) {
                auto action = popupMenu.addAction(tr("Remove \"%1\"").arg(shortcut.toString(QKeySequence::NativeText)));
                connect(action, &QAction::triggered, this, [=] {
                    removeItemShortcut(item, shortcut);
                });
            }
            popupMenu.exec(QCursor::pos());
        }

        void updateItemShortcuts(QTreeWidgetItem *item, const QList<QKeySequence> &shortcuts) {
            item->setData(0, ShortcutRole, QVariant::fromValue(shortcuts));
            QStringList shortcutTexts;
            for (const auto &shortcut : shortcuts) {
                shortcutTexts.append("[" + shortcut.toString(QKeySequence::NativeText) + "]");
            }
            if (m_presetComboBox->currentData().toBool()) {
                m_systemShortcutFamilyModification[m_presetComboBox->currentText()].insert(item->text(1), shortcuts);
            } else {
                m_userShortcutFamilyModification[m_presetComboBox->currentText()].insert(item->text(1), shortcuts);
            }
            item->setText(2, shortcutTexts.join(' '));
        }

        void promptShortcut(QTreeWidgetItem *item) {
            QDialog dlg(this);
            dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            dlg.setWindowTitle(tr("Add Shortcut for \"%1\"").arg(item->text(0)));
            auto mainLayout = new QVBoxLayout;
            auto keySequenceEdit = new QKeySequenceEdit;
            mainLayout->addWidget(keySequenceEdit);
            auto buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch();
            auto okButton = new QPushButton(tr("OK"));
            buttonLayout->addWidget(okButton);
            mainLayout->addLayout(buttonLayout);
            dlg.setLayout(mainLayout);

            connect(keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, &dlg, [=](const QKeySequence &keySequence) {
                if (m_shortcutInfo.contains(keySequence.toString())) {
                    okButton->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
                } else {
                    okButton->setIcon({});
                }
            });

            connect(okButton, &QAbstractButton::clicked, &dlg, [=, &dlg] {
                auto shortcut = keySequenceEdit->keySequence();
                if (m_shortcutInfo.contains(shortcut.toString())) {
                    auto ret = QMessageBox::warning(&dlg, tr("Shortcut Already Assigned"), tr("Shortcut \"%1\" has already been assigned to action \"%2\".\n\nIf continue, \"%1\" will be removed from \"%2\".").arg(shortcut.toString(QKeySequence::NativeText), m_shortcutInfo.value(shortcut.toString())->text(0)), QMessageBox::Ok | QMessageBox::Cancel);
                    if (ret == QMessageBox::Cancel)
                        return;
                    removeItemShortcut(m_shortcutInfo.value(shortcut.toString()), shortcut);
                }
                auto shortcuts = item->data(0, ShortcutRole).value<QList<QKeySequence>>();
                shortcuts.append(shortcut);
                m_shortcutInfo.insert(shortcut.toString(), item);
                updateItemShortcuts(item, shortcuts);
                dlg.accept();
            });

            dlg.exec();
        }

        void removeItemShortcut(QTreeWidgetItem *item, const QKeySequence &shortcut) {
            if (!item || item->data(0, ShortcutRole).isNull())
                return;
            auto shortcuts = item->data(0, ShortcutRole).value<QList<QKeySequence>>();
            if (shortcuts.removeOne(shortcut)) {
                m_shortcutInfo.remove(shortcut.toString());
                updateItemShortcuts(item, shortcuts);
            }
        }

        QString promptPresetName(const QString &title) {
            QDialog dlg(this);
            dlg.setWindowTitle(title);
            dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            auto mainLayout = new QVBoxLayout;
            auto lineEdit = new QLineEdit;
            mainLayout->addWidget(lineEdit);
            auto buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch();
            auto okButton = new QPushButton(tr("OK"));
            buttonLayout->addWidget(okButton);
            mainLayout->addLayout(buttonLayout);
            dlg.setLayout(mainLayout);

            auto userShortcutsFamilyList = ICore::instance()->actionManager()->userShortcutFamilies();
            QSet<QString> userShortcutsFamilies(userShortcutsFamilyList.cbegin(), userShortcutsFamilyList.cend());
            connect(lineEdit, &QLineEdit::textChanged, &dlg, [=](const QString &name) {
                if (name.isEmpty()) {
                    okButton->setDisabled(true);
                    okButton->setIcon({});
                } else if (userShortcutsFamilies.contains(name) || m_userShortcutFamilyAddition.contains(name)) {
                    okButton->setDisabled(false);
                    okButton->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
                } else {
                    okButton->setDisabled(false);
                    okButton->setIcon({});
                }
            });

            connect(okButton, &QAbstractButton::clicked, &dlg, [=, &dlg] {
                auto name = lineEdit->text();
                if (userShortcutsFamilies.contains(name) || m_userShortcutFamilyAddition.contains(name)) {
                    if (QMessageBox::warning(&dlg, tr("Name Already Exists"), tr("Preset name \"%1\" already exists.\n\nIf continue, the old one will be overwritten.").arg(name), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
                        return;
                }
                dlg.accept();
            });
            if (dlg.exec() == QDialog::Accepted)
                return lineEdit->text();
            return {};
        }

        [[nodiscard]] static QHash<QString, QList<QKeySequence>> duplicateShortcutsFamily(const ActionDomain::ShortcutsFamily &family, const QHash<QString, QList<QKeySequence>> &addition, const QHash<QString, QList<QKeySequence>> &modification, bool *changed = nullptr) {
            QHash<QString, QList<QKeySequence>> ret;
            if (changed)
                *changed = false;
            for (const auto &id : family.keys()) {
                if (family.value(id)) {
                    ret.insert(id, family.value(id).value());
                }
            }
            auto patchedModification = addition;
            patchedModification.insert(modification);
            for (const auto &id : patchedModification.keys()) {
                if (changed && !*changed) {
                    auto currentList = ret.value(id);
                    auto modifiedList = patchedModification.value(id);
                    if (QSet<QKeySequence>(currentList.cbegin(), currentList.cend()) != QSet<QKeySequence>(modifiedList.cbegin(), modifiedList.cend())) {
                        *changed = true;
                    }
                }
                ret.insert(id, patchedModification.value(id));
            }
            return ret;
        }

    };

    KeymapPage::KeymapPage(QObject *parent) : ISettingPage(QStringLiteral("core.Keymap"), parent) {
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
            m_widget = new KeymapPageWidget();
        }
        return m_widget;
    }

    bool KeymapPage::accept() {
        if (!m_widget)
            return true;
        m_widget->applyModification();
        return true;
    }

    void KeymapPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}

#include "keymappage.moc"