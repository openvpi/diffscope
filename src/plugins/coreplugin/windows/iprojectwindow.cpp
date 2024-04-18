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
        IProjectWindowPrivate() {
        }

        void init() {
            auto icore = ICore::instance();
            connect(icore, &ICore::actionLayoutsReloaded, this,
                    &IProjectWindowPrivate::reloadLayouts);
            connect(icore, &ICore::actionShortcutsReloaded, this,
                    &IProjectWindowPrivate::reloadShortcuts);
        }

        void reloadLayouts() {
            auto domain = ICore::instance()->actionDomain();
            auto arr = actionItemMap.values();
            domain->buildLayouts(qIDec->theme(), {arr.begin(), arr.end()});
        }

        void reloadShortcuts() {
            auto domain = ICore::instance()->actionDomain();
            for (const auto &item : std::as_const(actionItemMap)) {
                if (item->isAction()) {
                    item->action()->setShortcuts(domain->objectShortcuts(item->id()));
                }
            }
        }
    };

    IProjectWindow::IProjectWindow(QObject *parent)
        : IWindow(*new IProjectWindowPrivate(), QStringLiteral("project"), parent) {
        Q_D(IProjectWindow);
        d->init();
    }

    IProjectWindow::~IProjectWindow() {
    }

    QMenuBar *IProjectWindow::menuBar() const {
        return qobject_cast<Internal::ProjectWindow *>(window())->menuBar();
    }

    void IProjectWindow::setMenuBar(QMenuBar *menuBar) {
        qobject_cast<Internal::ProjectWindow *>(window())->setMenuBar(menuBar);
    }

    QWidget *IProjectWindow::centralWidget() const {
        return qobject_cast<Internal::ProjectWindow *>(window())->centralWidget();
    }

    void IProjectWindow::setCentralWidget(QWidget *widget) {
        qobject_cast<Internal::ProjectWindow *>(window())->setCentralWidget(widget);
    }

    QStatusBar *IProjectWindow::statusBar() const {
        return qobject_cast<Internal::ProjectWindow *>(window())->statusBar();
    }

    void IProjectWindow::setStatusBar(QStatusBar *statusBar) {
        qobject_cast<Internal::ProjectWindow *>(window())->setStatusBar(statusBar);
    }

    QString IProjectWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(AppExtra::displayTitle(title));
    }

    QWidget *IProjectWindow::createWindow(QWidget *parent) const {
        return new Internal::ProjectWindow(parent);
    }

    void IProjectWindow::nextLoadingState(Core::IWindow::State nextState) {
        Q_D(IProjectWindow);

        switch (nextState) {
            case IWindow::WindowSetup: {
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

}