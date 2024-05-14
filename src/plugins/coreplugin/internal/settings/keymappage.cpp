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
    public:
        explicit KeymapPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;

            auto presetLayout = new QFormLayout;
            auto presetRowLayout = new QHBoxLayout;
            auto presetComboBox = new QComboBox;
            presetRowLayout->addWidget(presetComboBox, 1);
            auto presetSaveAsButton = new QPushButton(tr("Save As"));
            presetRowLayout->addWidget(presetSaveAsButton);
            auto presetDeleteButton = new QPushButton(tr("Delete"));
            presetRowLayout->addWidget(presetDeleteButton);
            auto presetRenameButton = new QPushButton(tr("Rename"));
            presetRowLayout->addWidget(presetRenameButton);
            auto presetLabel = new QLabel(tr("Preset"));
            presetLabel->setBuddy(presetComboBox);
            presetLayout->addRow(presetLabel, presetRowLayout);
            mainLayout->addLayout(presetLayout);

            auto actionsGroupBox = new QGroupBox(tr("Actions"));
            auto actionsLayout = new QVBoxLayout;
            auto actionsFilterLayout = new QHBoxLayout;
            auto expandAllButton = new QPushButton(tr("Expand All"));
            actionsFilterLayout->addWidget(expandAllButton);
            auto collapseAllButton = new QPushButton(tr("Collapse All"));
            actionsFilterLayout->addWidget(collapseAllButton);
            auto filterLineEdit = new QLineEdit;
            filterLineEdit->setPlaceholderText(tr("Filter"));
            actionsFilterLayout->addWidget(filterLineEdit, 1);
            auto findShortcutButton = new QPushButton(tr("Find Shortcut"));
            actionsFilterLayout->addWidget(findShortcutButton);
            actionsLayout->addLayout(actionsFilterLayout);
            auto actionsTreeWidget = new QTreeWidget;
            actionsTreeWidget->setHeaderLabels({tr("Name"), tr("ID"), tr("Shortcut")});
            actionsLayout->addWidget(actionsTreeWidget);
            actionsGroupBox->setLayout(actionsLayout);
            mainLayout->addWidget(actionsGroupBox);

            setLayout(mainLayout);

            for (const auto &catalog : ICore::instance()->actionManager()->domain()->catalog().children()) {
                auto item = new QTreeWidgetItem;
                traverseCatalog(item, catalog);
                actionsTreeWidget->addTopLevelItem(item);
            }
            actionsTreeWidget->expandAll();
            actionsTreeWidget->resizeColumnToContents(0);
            actionsTreeWidget->resizeColumnToContents(1);
            actionsTreeWidget->collapseAll();

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
                dlg.move(QCursor::pos());
                dlg.exec();
            });

            connect(actionsTreeWidget, &QTreeWidget::itemActivated, this, &KeymapPageWidget::handleItemContextMenu);
            actionsTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
            actionsTreeWidget->installEventFilter(this);
            connect(actionsTreeWidget, &QTreeWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
                auto *item = actionsTreeWidget->itemAt(pos);
                handleItemContextMenu(item);
            });

        }

    private:
        enum ItemDataRole {
            ShortcutRole = Qt::UserRole,
        };

        QHash<QString, QTreeWidgetItem *> m_shortcutInfo;
        QList<QTreeWidgetItem *> m_modifiedItems;

        void traverseCatalog(QTreeWidgetItem *item, const ActionCatalog &catalog) {
            auto domain = ICore::instance()->actionManager()->domain();
            item->setText(0, ActionObjectInfo::translatedCategory(catalog.name()));
            item->setIcon(0, domain->objectIcon(qIDec->theme(), catalog.id()));
            item->setText(1, catalog.id());
            for (const auto &childCatalog : catalog.children()) {
                auto childItem = new QTreeWidgetItem;
                traverseCatalog(childItem, childCatalog);
                item->addChild(childItem);
            }
            if (!catalog.children().isEmpty())
                return;
            QStringList shortcutTexts;
            QVariantList shortcutKeys;
            for (const auto &keySeq : domain->objectShortcuts(catalog.id())) {
                shortcutTexts.append("[" + keySeq.toString(QKeySequence::NativeText) + "]");
                shortcutKeys.append(keySeq.toString());
                m_shortcutInfo.insert(keySeq.toString(), item);
            }
            item->setText(2, shortcutTexts.join(' '));
            item->setData(0, ShortcutRole, shortcutKeys);
        }

        bool traverseFilter(QTreeWidgetItem *item, const QString &text) {
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
            for (const auto &shortcutKey : item->data(0, ShortcutRole).toStringList()) {
                QKeySequence shortcut(shortcutKey);
                auto action = popupMenu.addAction(tr("Remove \"%1\"").arg(shortcut.toString(QKeySequence::NativeText)));
                connect(action, &QAction::triggered, this, [=] {
                    removeItemShortcut(item, shortcut);
                });
            }
            popupMenu.exec(QCursor::pos());
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
                auto shortcutKeys = item->data(0, ShortcutRole).toStringList();
                shortcutKeys.append(shortcut.toString());
                m_shortcutInfo.insert(shortcut.toString(), item);
                item->setData(0, ShortcutRole, shortcutKeys);
                m_modifiedItems.append(item);
                QStringList shortcutTexts;
                for (const auto &shortcutKey : shortcutKeys) {
                    shortcutTexts.append("[" + QKeySequence(shortcutKey).toString(QKeySequence::NativeText) + "]");
                }
                item->setText(2, shortcutTexts.join(' '));
                dlg.accept();
            });

            dlg.exec();
        }

        void removeItemShortcut(QTreeWidgetItem *item, const QKeySequence &shortcut) {
            if (!item || item->data(0, ShortcutRole).isNull())
                return;
            auto shortcutKeys = item->data(0, ShortcutRole).toStringList();
            if (shortcutKeys.removeOne(shortcut.toString())) {
                m_shortcutInfo.remove(shortcut.toString());
                item->setData(0, ShortcutRole, shortcutKeys);
                m_modifiedItems.append(item);
                QStringList shortcutTexts;
                for (const auto &shortcutKey : shortcutKeys) {
                    shortcutTexts.append("[" + QKeySequence(shortcutKey).toString(QKeySequence::NativeText) + "]");
                }
                item->setText(2, shortcutTexts.join(' '));
            }
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
        return true;
    }

    void KeymapPage::finish() {
        if (m_widget) {
            m_widget->deleteLater();
            m_widget = nullptr;
        }
    }

}
