#include "appextra.h"

#include <QtCore/QTimer>
#include <QtWidgets/QStyle>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QApplication>

#include <QMWidgets/cmenu.h>

namespace {

    class PopUpMenuWatcher : public QObject {
    public:
        explicit PopUpMenuWatcher(QWidget *parent = nullptr) : QObject(parent) {
            parent->installEventFilter(this);
        }

        static void polishMenu(QObject *child) {
            QMenu *menu;
            if ((menu = qobject_cast<QMenu *>(child))) {
                menu->setProperty("core-style", true);
                menu->style()->polish(menu);

                // Why need to place after polish?
                menu->setFont(QApplication::font());
            }
        }

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (event->type() == QEvent::ChildAdded) {
                auto e = static_cast<QChildEvent *>(event);
                auto child = e->child();
                QTimer::singleShot(0, child, [child]() {
                    polishMenu(child); //
                });
            }
            return QObject::eventFilter(obj, event);
        }
    };

}

namespace Core {

    static const char *coreMenuStyleName = "core-style";

    QMenu *AppExtra::createCoreMenu(QWidget *parent) {
        auto menu = new CMenu(parent);
        return menu;
    }

    void AppExtra::autoPolishPopupMenu(QWidget *w) {
        new PopUpMenuWatcher(w);
    }

    void AppExtra::autoPolishScrollArea(QWidget *w) {
        auto polishScrollArea = [](QAbstractScrollArea *area) {
            area->setProperty(coreMenuStyleName, true);
            autoPolishScrollBars(area);
        };
        if (auto area = qobject_cast<QAbstractScrollArea *>(w)) {
            polishScrollArea(area);
            return;
        }
        for (const auto &area : w->findChildren<QAbstractScrollArea *>()) {
            polishScrollArea(area);
        }
    }

    void AppExtra::autoPolishScrollBars(QWidget *w) {
        for (const auto &bar : w->findChildren<QScrollBar *>()) {
            bar->setProperty(coreMenuStyleName, true);
            autoPolishPopupMenu(bar);
        }
    }

    QString AppExtra::fileManagerName() {
#ifdef Q_OS_WINDOWS
        return QCoreApplication::translate("QMSystem", "Explorer");
#elif defined(Q_OS_MAC)
        return QCoreApplication::translate("QMSystem", "Finder");
#else
        return QCoreApplication::translate("QMSystem", "File Manager");
#endif
    }

}