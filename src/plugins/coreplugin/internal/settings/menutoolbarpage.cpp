#include "menutoolbarpage.h"

#include <list>
#include <utility>
#include <memory>

#include <QBoxLayout>
#include <QToolBar>
#include <QLineEdit>
#include <QStyle>
#include <QDebug>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QDropEvent>

#include <QMWidgets/ctreewidget.h>

#include <QMWidgets/qmdecoratorv2.h>

#include <coreplugin/icore.h>

namespace Core {

    class MenuToolbarPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MenuToolbarPageWidget(QWidget *parent = nullptr) {
            auto mainLayout = new QVBoxLayout;

            auto toolbar = new QToolBar;
            mainLayout->addWidget(toolbar);

            m_treeWidget = new CTreeWidget;
            m_treeWidget->setColumnCount(2);
            m_treeWidget->setColumnWidth(0, 400);
            m_treeWidget->setHeaderLabels({tr("Name"), tr("Type")});
            m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
            m_treeWidget->setDragEnabled(true);
            m_treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
            m_treeWidget->invisibleRootItem()->setFlags(m_treeWidget->invisibleRootItem()->flags() & ~Qt::ItemIsDropEnabled);
            mainLayout->addWidget(m_treeWidget);

            setLayout(mainLayout);

            m_addActionDialog = new QDialog(this);
            m_addActionDialog->setWindowTitle(tr("Add Action"));
            m_addActionDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            auto addActionDialogLayout = new QVBoxLayout;
            m_actionCatalogTreeWidget = new QTreeWidget;
            m_actionCatalogTreeWidget->setHeaderHidden(true);
            addActionDialogLayout->addWidget(m_actionCatalogTreeWidget);
            auto specFormLayout = new QFormLayout;
            // TODO icon
            addActionDialogLayout->addLayout(specFormLayout);
            auto buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch();
            auto okButton = new QPushButton(tr("OK"));
            okButton->setDisabled(true);
            buttonLayout->addWidget(okButton);
            auto cancelButton = new QPushButton(tr("Cancel"));
            buttonLayout->addWidget(cancelButton);
            addActionDialogLayout->addLayout(buttonLayout);
            m_addActionDialog->setLayout(addActionDialogLayout);

            for (const auto &childCatalog : ICore::instance()->actionManager()->domain()->catalog().children()) {
                auto item = new QTreeWidgetItem;
                m_actionCatalogTreeWidget->addTopLevelItem(item);
                setItemByCatalog(item, childCatalog);
                if (!childCatalog.children().isEmpty())
                    initializeLazyLoad(item);
            }

            connect(m_actionCatalogTreeWidget, &QTreeWidget::itemExpanded, this, &MenuToolbarPageWidget::commitCatalogLazyLoad);

            connect(okButton, &QAbstractButton::clicked, m_addActionDialog, &QDialog::accept);
            connect(cancelButton, &QAbstractButton::clicked, m_addActionDialog, &QDialog::reject);
            connect(m_actionCatalogTreeWidget, &QTreeWidget::currentItemChanged, m_addActionDialog, [=](QTreeWidgetItem *item) {
                okButton->setEnabled(static_cast<bool>(item));
            });

            {
                auto addActionAction = toolbar->addAction(style()->standardIcon(QStyle::SP_DialogApplyButton), tr("Add &Action"), this, &MenuToolbarPageWidget::handleAddAction);
                addActionAction->setShortcut(Qt::ALT + Qt::Key_Insert);
                addActionAction->setDisabled(true);
                auto addSeparatorAction = toolbar->addAction(style()->standardIcon(QStyle::SP_DialogYesButton), tr("Add &Separator"), this, &MenuToolbarPageWidget::handleAddSeparator);
                addSeparatorAction->setShortcut(Qt::ALT + Qt::SHIFT + Qt::Key_Insert);
                addSeparatorAction->setDisabled(true);
                auto removeAction = toolbar->addAction(style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("&Remove"), this, &MenuToolbarPageWidget::handleRemove);
                removeAction->setShortcut(Qt::ALT + Qt::Key_Delete);
                removeAction->setDisabled(true);
                auto editIconAction = toolbar->addAction(style()->standardIcon(QStyle::SP_DialogResetButton), tr("&Edit Icon"), this, &MenuToolbarPageWidget::handleEditIcon);
                editIconAction->setShortcut(Qt::Key_F4);
                editIconAction->setDisabled(true);
                auto moveUpAction = toolbar->addAction(style()->standardIcon(QStyle::SP_ArrowUp), tr("Move &Up"), this, &MenuToolbarPageWidget::handleMoveUp);
                moveUpAction->setShortcut(Qt::ALT + Qt::Key_Up);
                moveUpAction->setDisabled(true);
                auto moveDownAction = toolbar->addAction(style()->standardIcon(QStyle::SP_ArrowDown), tr("Move &Down"), this, &MenuToolbarPageWidget::handleMoveDown);
                moveDownAction->setShortcut(Qt::ALT + Qt::Key_Down);
                moveDownAction->setDisabled(true);
                toolbar->addSeparator();
                auto searchLineEdit = new QLineEdit;
                searchLineEdit->setPlaceholderText(tr("Find"));
                toolbar->addWidget(searchLineEdit);

                connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
                    auto item = m_treeWidget->currentItem();
                    QMenu popupMenu(this);
                    popupMenu.addActions({addActionAction, addSeparatorAction, removeAction, editIconAction, moveUpAction, moveDownAction});
                    if (!item || !item->data(0, TopLevelRole).toBool()) {
                        popupMenu.addSeparator();
                        auto itemType = item->data(0, TypeRole).toInt();
                        if (itemType & ActionLayoutInfo::PlaceHolderFlag) {
                            auto a1 = popupMenu.addAction(tr("Set as Se&parator"), [=] { setItemType(item, ActionLayoutInfo::Separator); });
                            a1->setCheckable(true);
                            a1->setChecked(itemType == ActionLayoutInfo::Separator);
                            auto a2 = popupMenu.addAction(tr("Set as S&tretch"), [=] { setItemType(item, ActionLayoutInfo::Stretch); });
                            a2->setCheckable(true);
                            a2->setChecked(itemType == ActionLayoutInfo::Stretch);
                        } else if (itemType == ActionLayoutInfo::ExpandedMenu || itemType == ActionLayoutInfo::Menu) {
                            auto a1 = popupMenu.addAction(tr("Set as &Group"), [=] { setItemType(item, ActionLayoutInfo::ExpandedMenu); });
                            a1->setCheckable(true);
                            a1->setChecked(itemType == ActionLayoutInfo::ExpandedMenu);
                            auto a2 = popupMenu.addAction(tr("Set as &Menu"), [=] { setItemType(item, ActionLayoutInfo::Menu); });
                            a2->setCheckable(true);
                            a2->setChecked(itemType == ActionLayoutInfo::Menu);
                        }
                    }
                    popupMenu.exec(QCursor::pos());
                });
                connect(m_treeWidget, &QTreeWidget::currentItemChanged, this, [=](QTreeWidgetItem *item) {
                    if (item->data(0, TopLevelRole).toBool() || !item->parent()) {
                        addActionAction->setDisabled(false);
                        addSeparatorAction->setDisabled(false);
                        removeAction->setDisabled(true);
                        editIconAction->setDisabled(true);
                        moveUpAction->setDisabled(true);
                        moveDownAction->setDisabled(true);
                    } else {
                        addActionAction->setDisabled(false);
                        addSeparatorAction->setDisabled(false);
                        removeAction->setDisabled(false);
                        editIconAction->setDisabled(false);
                        moveUpAction->setDisabled(item->parent()->indexOfChild(item) == 0);
                        moveDownAction->setDisabled(item->parent()->indexOfChild(item) == item->parent()->childCount() - 1);
                    }
                });
            }

            for (const auto &layout : ICore::instance()->actionManager()->domain()->layouts()) {
                auto item = new QTreeWidgetItem;
                m_treeWidget->addTopLevelItem(item);
                item->setData(0, TopLevelRole, true);
                setItemByLayout(item, layout);
                if (!(layout.type() & ActionLayoutInfo::TerminalFlag) && !layout.children().isEmpty())
                    initializeLazyLoad(item);
            }

            connect(m_treeWidget, &QTreeWidget::itemExpanded, this, &MenuToolbarPageWidget::commitLayoutLazyLoad);
            connect(m_treeWidget, &CTreeWidget::itemDropped, this, [=](QTreeWidgetItem *item) {
                commitLayoutLazyLoad(item);
                markDirty(item);
            });
        }

        ~MenuToolbarPageWidget() override {
            std::for_each(m_cachedActionLayouts.cbegin(), m_cachedActionLayouts.cend(), std::default_delete<ActionLayout>());
            std::for_each(m_cachedActionCatalogs.cbegin(), m_cachedActionCatalogs.cend(), std::default_delete<ActionCatalog>());
        }

        void applyModification() {
            auto rootLayout = makeLayoutFromItem(m_treeWidget->invisibleRootItem());
            ICore::instance()->actionManager()->domain()->setLayouts(rootLayout.children());
        }

    private:
        CTreeWidget *m_treeWidget;
        QDialog *m_addActionDialog;
        QTreeWidget *m_actionCatalogTreeWidget;
        mutable QSet<ActionLayout *> m_cachedActionLayouts;
        mutable QSet<ActionCatalog *> m_cachedActionCatalogs;

        enum DataRole {
            IDRole = Qt::UserRole,
            TypeRole,
            CacheRole,
            TopLevelRole,
            LazyLoadRole,
        };

        enum LazyLoadState {
            NotLoaded,
            Loaded,
            Placeholder,
        };

        static void setItemTypeColumn(QTreeWidgetItem *item) {
            static auto seperatorText = QStringLiteral("------------");
            if (item->data(0, TopLevelRole).toBool()) {
                item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);
                return;
            }
            auto itemType = item->data(0, TypeRole).toInt();
            if (itemType & ActionLayoutInfo::TerminalFlag) {
                item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);
            }
            switch (itemType) {
                case ActionLayoutInfo::Action:
                    item->setText(1, tr("Action"));
                    break;
                case ActionLayoutInfo::ExpandedMenu:
                case ActionLayoutInfo::Group:
                    item->setText(1, tr("Group"));
                    break;
                case ActionLayoutInfo::Menu:
                    item->setText(1, tr("Menu"));
                    break;
                case ActionLayoutInfo::Separator:
                    item->setText(0, seperatorText);
                    item->setText(1, tr("Separator"));
                    break;
                case ActionLayoutInfo::Stretch:
                    item->setText(0, seperatorText);
                    item->setText(1, tr("Stretch"));
                    break;
            }
        }

        void setItemByLayout(QTreeWidgetItem *item, const ActionLayout &layout) const {
            auto domain = ICore::instance()->actionManager()->domain();
            item->setText(0, ActionObjectInfo::translatedText(domain->objectInfo(layout.id()).text()));
            item->setIcon(0, domain->objectIcon(qIDec->theme(), layout.id()));
            item->setData(0, IDRole, layout.id());
            item->setData(0, TypeRole, layout.type());
            auto cachedLayout = new ActionLayout(layout);
            m_cachedActionLayouts.insert(cachedLayout);
            item->setData(0, CacheRole, reinterpret_cast<qintptr>(cachedLayout));
            setItemTypeColumn(item);
        }

        static void initializeLazyLoad(QTreeWidgetItem *item) {
            item->setData(0, LazyLoadRole, NotLoaded);
            auto placeHolder = new QTreeWidgetItem;
            placeHolder->setData(0, LazyLoadRole, Placeholder);
            item->addChild(placeHolder);
        }

        void commitLayoutLazyLoad(QTreeWidgetItem *item) const {
            if (item->data(0, LazyLoadRole) == Loaded)
                return;
            Q_ASSERT(item->child(0)->data(0, LazyLoadRole) == Placeholder);
            auto cachedLayout = reinterpret_cast<ActionLayout *>(item->data(0, CacheRole).value<qintptr>());
            QList<QTreeWidgetItem *> childItems;
            childItems.reserve(cachedLayout->children().size());
            for (const auto &childLayout : cachedLayout->children()) {
                auto childItem = new QTreeWidgetItem;
                setItemByLayout(childItem, childLayout);
                if (!(childLayout.type() & ActionLayoutInfo::TerminalFlag) && !childLayout.children().isEmpty())
                    initializeLazyLoad(childItem);
                childItems.append(childItem);
            }
            delete item->child(0);
            item->insertChildren(0, childItems);
            item->setData(0, LazyLoadRole, Loaded);
        }

        void setItemByCatalog(QTreeWidgetItem *item, const ActionCatalog &catalog) const {
            auto domain = ICore::instance()->actionManager()->domain();
            item->setText(0, ActionObjectInfo::translatedCategory(catalog.name()));
            item->setIcon(0, domain->objectIcon(qIDec->theme(), catalog.id()));
            item->setData(0, IDRole, catalog.id());
            auto cachedCatalog = new ActionCatalog(catalog);
            m_cachedActionCatalogs.insert(cachedCatalog);
            item->setData(0, CacheRole, reinterpret_cast<qintptr>(cachedCatalog));
        }

        void commitCatalogLazyLoad(QTreeWidgetItem *item) const {
            if (item->data(0, LazyLoadRole) != NotLoaded)
                return;
            Q_ASSERT(item->child(0)->data(0, LazyLoadRole) == Placeholder);
            auto cachedCatalog = reinterpret_cast<ActionCatalog *>(item->data(0, CacheRole).value<qintptr>());
            QList<QTreeWidgetItem *> childItems;
            childItems.reserve(cachedCatalog->children().size());
            for (const auto &childCatalog : cachedCatalog->children()) {
                auto childItem = new QTreeWidgetItem;
                setItemByCatalog(childItem, childCatalog);
                if (!childCatalog.children().isEmpty())
                    initializeLazyLoad(childItem);
                childItems.append(childItem);
            }
            delete item->child(0);
            m_cachedActionCatalogs.remove(cachedCatalog);
            delete cachedCatalog;
            item->insertChildren(0, childItems);
            item->setData(0, LazyLoadRole, Loaded);
        }

        void traverseCatalogItemAndModifyToLayoutItem(QTreeWidgetItem *item) const {
            auto domain = ICore::instance()->actionManager()->domain();
            auto id = item->data(0, IDRole).toString();
            item->setText(0, ActionObjectInfo::translatedText(domain->objectInfo(id).text()));
            item->setData(0, TypeRole, [=] {
                switch(domain->objectInfo(id).type()) {
                    case ActionObjectInfo::Group:
                        return ActionLayoutInfo::Group;
                    case ActionObjectInfo::Menu:
                        return ActionLayoutInfo::Menu;
                    default:
                        return ActionLayoutInfo::Action;
                }
            }());
            setItemTypeColumn(item);
            commitCatalogLazyLoad(item);
            for (int i = 0; i < item->childCount(); i++) {
                traverseCatalogItemAndModifyToLayoutItem(item->child(i));
            }
        }

        ActionLayout makeLayoutFromItem(QTreeWidgetItem *item) const {
            auto layout = reinterpret_cast<ActionLayout *>(item->data(0, CacheRole).value<qintptr>());
            if (layout)
                return *layout;
            layout = new ActionLayout;
            layout->setId(item->data(0, IDRole).toString());
            layout->setType(static_cast<ActionLayoutInfo::Type>(item->data(0, TypeRole).toInt()));
            QList<ActionLayout> childLayouts;
            for (int i = 0; i < item->childCount(); i++) {
                childLayouts.append(makeLayoutFromItem(item->child(i)));
            }
            layout->setChildren(childLayouts);
            m_cachedActionLayouts.insert(layout);
            item->setData(0, CacheRole, reinterpret_cast<qintptr>(layout));
            return *layout;
        }

        void markDirty(QTreeWidgetItem *item) const {
            for (; item && !item->data(0, CacheRole).isNull(); item = item->parent()) {
                auto cachedLayout = reinterpret_cast<ActionLayout *>(item->data(0, CacheRole).value<qintptr>());
                item->setData(0, CacheRole, {});
                m_cachedActionLayouts.remove(cachedLayout);
                delete cachedLayout;
            }
        }

        void setItemType(QTreeWidgetItem *item, ActionLayoutInfo::Type type) const {
            if (item->data(0, TypeRole) != type) {
                item->setData(0, TypeRole, type);
                setItemTypeColumn(item);
                markDirty(item);
            }
        }

        void handleAddAction() {
            m_actionCatalogTreeWidget->setCurrentItem(nullptr);
            m_actionCatalogTreeWidget->collapseAll();
            if (m_addActionDialog->exec() == QDialog::Rejected)
                return;
            auto actionItem = m_actionCatalogTreeWidget->currentItem()->clone();
            traverseCatalogItemAndModifyToLayoutItem(actionItem);
            auto item = m_treeWidget->currentItem();
            if (!(item->data(0, TypeRole).toInt() & ActionLayoutInfo::TerminalFlag)) {
                item->setExpanded(true);
                item->addChild(actionItem);
                m_treeWidget->setCurrentItem(actionItem);
                markDirty(item);
            } else {
                item->parent()->insertChild(item->parent()->indexOfChild(item) + 1, actionItem);
                m_treeWidget->setCurrentItem(actionItem);
                markDirty(item->parent());
            }
        }

        void handleAddSeparator() {
            auto separatorItem = new QTreeWidgetItem;
            separatorItem->setData(0, TypeRole, ActionLayoutInfo::Separator);
            auto item = m_treeWidget->currentItem();
            if (item->childCount()) {
                item->setExpanded(true);
                item->addChild(separatorItem);
                m_treeWidget->setCurrentItem(separatorItem);
                markDirty(item);
            } else {
                item->parent()->insertChild(item->parent()->indexOfChild(item) + 1, separatorItem);
                m_treeWidget->setCurrentItem(separatorItem);
                markDirty(item->parent());
            }
            setItemTypeColumn(separatorItem);
        }

        void handleRemove() {
            delete m_treeWidget->currentItem();
            markDirty(m_treeWidget->currentItem()->parent());
        }

        void handleEditIcon() {

        }

        void handleMoveUp() {
            auto item = m_treeWidget->currentItem();
            auto parentItem = item->parent();
            auto index = parentItem->indexOfChild(item);
            parentItem->takeChild(index);
            parentItem->insertChild(index - 1, item);
            setItemTypeColumn(item);
            m_treeWidget->setCurrentItem(item);
            markDirty(parentItem);
        }

        void handleMoveDown() {
            auto item = m_treeWidget->currentItem();
            auto parentItem = item->parent();
            auto index = parentItem->indexOfChild(item);
            parentItem->takeChild(index);
            parentItem->insertChild(index + 1, item);
            setItemTypeColumn(item);
            m_treeWidget->setCurrentItem(item);
            markDirty(parentItem);
        }

    };

    MenuToolbarPage::MenuToolbarPage(QObject *parent) : ISettingPage(QStringLiteral("core.MenuToolbar"), parent) {
        setTitle([] { return tr("Menu and Toolbar"); });
        setDescription([] { return tr("Configure the layout of menus and toolbars"); });
    }

    MenuToolbarPage::~MenuToolbarPage() = default;

    QString MenuToolbarPage::sortKeyword() const {
        return QStringLiteral("MenuToolbar");
    }
    bool MenuToolbarPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }
    QWidget *MenuToolbarPage::widget() {
        if (m_widget)
            return m_widget;
        m_widget = new MenuToolbarPageWidget;
        return m_widget;
    }
    bool MenuToolbarPage::accept() {
        m_widget->applyModification();
        return true;
    }
    void MenuToolbarPage::finish() {
    }
} // Core

#include "menutoolbarpage.moc"