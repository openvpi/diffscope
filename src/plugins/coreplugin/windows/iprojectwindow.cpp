#include "iprojectwindow.h"

#include <QDebug>
#include <QMenuBar>

#include <CoreApi/private/iwindow_p.h>

#include "appextra.h"
#include "icore.h"
#include "projectwindow.h"

namespace Core {

    namespace Internal {
        extern ActionDomain *projectActionDomain;
    }

    class IProjectWindowPrivate : public IWindowPrivate {
        Q_DECLARE_PUBLIC(IProjectWindow)
    public:
        void reloadMenuBar() {
            Q_Q(IProjectWindow);
        }
    };

    IProjectWindow::IProjectWindow(QObject *parent)
        : IWindow(*new IProjectWindowPrivate(), QStringLiteral("project"), parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

    ActionDomain *IProjectWindow::actionDomain() {
        return Internal::projectActionDomain;
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
                auto win = window();

                // Add window and menubar as basic shortcut contexts
                addShortcutContext(menuBar(), IWindow::Stable);

                //                auto actionMgr = ICore::instance()->actionSystem();
                //                d->mainMenuDomain = actionMgr->domain(QStringLiteral("project"));
                //                if (!d->mainMenuDomain) {
                //                    AppExtra::fatalError(win, tr("Failed to create main menu."));
                //                }

                //                d->invisibleCtx = ICore::instance()->actionSystem()->context(
                //                    QString("%1.InvisibleContext").arg(id()));
                //                if (!d->invisibleCtx) {
                //                    ICore::fatalError(win, tr("Failed to create internal action
                //                    context."));
                //                }

                // Init command palette
                //                d->cp = new CommandPalette(win);

                win->installEventFilter(d);
                break;
            }

            case IWindow::Initialized: {
                //                connect(d->mainMenuDomain, &ActionDomain::stateChanged, d,
                //                        &IProjectWindowPrivate::reloadMenuBar);
                //                d->reloadMenuBar();
                //                d->loadInvisibleContext();
                break;
            }

            default:
                break;
        }
    }
}