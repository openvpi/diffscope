#include "iprojectwindow.h"

#include <QDebug>
#include <QMenuBar>

#include <QMWidgets/qmdecoratorv2.h>

#include <CoreApi/private/iwindow_p.h>

#include "appextra.h"
#include "icore.h"
#include "projectwindow.h"

namespace Core {

    class IProjectWindowPrivate : public IWindowPrivate {
        Q_DECLARE_PUBLIC(IProjectWindow)
    public:
        IProjectWindowPrivate(IProjectWindow::Mode mode) : mode(mode) {
        }

        void init() {
            auto actionMgr = ICore::instance()->actionManager();
            connect(actionMgr, &ActionManager::layoutsReloaded, this,
                    &IProjectWindowPrivate::reloadLayouts);
            connect(actionMgr, &ActionManager::shortcutsReloaded, this,
                    &IProjectWindowPrivate::reloadShortcuts);
        }

        void reloadLayouts() {
            auto domain = ICore::instance()->actionManager()->domain();
            auto actions = actionItemMap.values_qlist();
            domain->buildLayouts(actions, AppExtra::createCoreMenu);
            domain->updateTexts(actions);
            domain->updateIcons(qIDec->theme(), actions);
        }

        void reloadShortcuts() {
            auto domain = ICore::instance()->actionManager()->domain();
            for (const auto &item : std::as_const(actionItemMap)) {
                if (item->isAction()) {
                    item->action()->setShortcuts(domain->objectShortcuts(item->id()));
                }
            }
        }

        void reloadStrings() {
            auto domain = ICore::instance()->actionManager()->domain();
            domain->updateTexts(actionItemMap.values_qlist());
        }

        IProjectWindow::Mode mode;
    };

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(Standalone, parent) {
    }

    IProjectWindow::IProjectWindow(IProjectWindow::Mode mode, QObject *parent)
        : IWindow(*new IProjectWindowPrivate(mode), parent) {
        Q_D(IProjectWindow);
        d->init();
    }

    IProjectWindow::Mode IProjectWindow::mode() const {
        Q_D(const IProjectWindow);
        return d->mode;
    }

    bool IProjectWindow::eventFilter(QObject *obj, QEvent *event) {
        return QObject::eventFilter(obj, event);
    }

    QMenuBar *IProjectWindow::menuBar() const {
        return static_cast<Internal::ProjectWindow *>(window())->menuBar();
    }

    void IProjectWindow::setMenuBar(QMenuBar *menuBar) {
        static_cast<Internal::ProjectWindow *>(window())->setMenuBar(menuBar);
    }

    QWidget *IProjectWindow::centralWidget() const {
        return static_cast<Internal::ProjectWindow *>(window())->centralWidget();
    }

    void IProjectWindow::setCentralWidget(QWidget *widget) {
        static_cast<Internal::ProjectWindow *>(window())->setCentralWidget(widget);
    }

    QStatusBar *IProjectWindow::statusBar() const {
        return static_cast<Internal::ProjectWindow *>(window())->statusBar();
    }

    void IProjectWindow::setStatusBar(QStatusBar *statusBar) {
        static_cast<Internal::ProjectWindow *>(window())->setStatusBar(statusBar);
    }

    QToolBar *IProjectWindow::mainToolbar() const {
        return static_cast<Internal::ProjectWindow *>(window())->toolBar();
    }

    IProjectWindow::~IProjectWindow() {
    }

    QString IProjectWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(AppExtra::displayTitle(title));
    }

    QWidget *IProjectWindow::createWindow(QWidget *parent) const {
        return new Internal::ProjectWindow(parent);
    }

    void IProjectWindow::nextLoadingState(State nextState) {
        Q_D(IProjectWindow);

        switch (nextState) {
            case IWindow::Preparatory: {
                // Install before all addons
                qIDec->installLocale(this, d);
                break;
            }

            case IWindow::Initialized: {
                d->reloadLayouts();
                d->reloadShortcuts();
                break;
            }

            default:
                break;
        }
    }

    IProjectWindowRegistry *IProjectWindowRegistry::instance() {
        static IProjectWindowRegistry reg;
        return &reg;
    }

}