#include "appextra.h"

#include <QtCore/QTimer>
#include <QtGui/QScreen>
#include <QtWidgets/QStyle>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>

#include <QMCore/qmsystem.h>
#include <QMWidgets/cmenu.h>

#include <choruskit_config.h>
#include <choruskit_buildinfo.h>

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

    static constexpr const char *coreMenuStyleName = "core-style";

    QString AppExtra::mainTitle() {
        return QString("%1 %2").arg(QApplication::organizationName(),
                                    QApplication::applicationName());
    }

    QString AppExtra::displayTitle(const QString &text) {
        return QString("%1 - %2").arg(text, mainTitle());
    }

    void AppExtra::aboutApp(QWidget *parent) {
        static const QString appName = qApp->applicationName();
        QString copyrightInfo =
            QApplication::translate("Application",
                                    "<p>Based on Qt version %1.<br>"
                                    "Copyright 2019-%2 Team OpenVPI. All rights reserved.</p>")
                .arg(QStringLiteral(QT_VERSION_STR), QStringLiteral(APPLICATION_BUILD_YEAR));

        QString buildInfo = QApplication::translate("Application", "<h3>Build Information</h3>"
                                                                   "<p>"
                                                                   "Version: %1<br>"
                                                                   "Branch: %2<br>"
                                                                   "Commit: %3<br>"
                                                                   "Build date: %4<br>"
                                                                   "Toolchain: %5 %6 %7"
                                                                   "</p>")
                                .arg(QApplication::applicationVersion(),
                                     QStringLiteral(APPLICATION_GIT_BRANCH),           //
                                     QStringLiteral(APPLICATION_GIT_LAST_COMMIT_HASH), //
                                     QStringLiteral(APPLICATION_BUILD_TIME),           //
                                     QStringLiteral(APPLICATION_COMPILER_ARCH),        //
                                     QStringLiteral(APPLICATION_COMPILER_ID),          //
                                     QStringLiteral(APPLICATION_COMPILER_VERSION));

        QString aboutInfo =
            QApplication::translate(
                "Application",
                "<h3>About Application</h3>"
                "<p>%1 is a cross-platform SVS editing application mainly powered by "
                "DiffSinger for virtual singer producers to make song compositions.</p>")
                .arg(appName);

        QString licenseInfo =
            QApplication::translate(
                "Application",
                "<h3>License</h3>"
                "<p>Licensed under the Apache License, Version 2.0.<br>"
                R"(You may obtain a copy of the License at %1.</p>)"
                "<p>This application is distributed "
                "<b>AS IS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND</b>, either express or "
                "implied.</p>")
                .arg(QStringLiteral(
                    R"(<a href="https://www.apache.org/licenses/LICENSE-2.0">apache.org/licenses</a>)"));

        QString translatedTextAboutQtCaption =
            QApplication::translate("Application", "<h2>%1</h2>%2%3%4%5")
                .arg(appName, copyrightInfo, buildInfo, aboutInfo, licenseInfo);

        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(
            QApplication::translate("Application", "About %1").arg(qApp->applicationName()));
        msgBox.setText(translatedTextAboutQtCaption);

        double ratio = (msgBox.screen()->logicalDotsPerInch() / QM::unitDpi());

        QIcon icon(qApp->windowIcon());
        if (!icon.isNull())
            msgBox.setIconPixmap(icon.pixmap(QSize(40, 40) * ratio));

        auto layout = qobject_cast<QGridLayout *>(msgBox.layout());
        auto horizontalSpacer =
            new QSpacerItem(ratio * 500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        msgBox.exec();
    }

    void AppExtra::fatalError(QWidget *parent, const QString &text, int exitCode) {
        QMessageBox msgBox(QMessageBox::Critical,
                           QApplication::translate("Application", "Fatal Error"), text,
                           QMessageBox::Ok, parent);
        msgBox.setText(
            QApplication::translate(
                "Application",
                "%1\n\nThe application is about to exit because an unresolvable "
                "exception has occurred, which may be caused by an illegal operation of an plugin.")
                .arg(text));
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        std::exit(exitCode);
    }

    QMenu *AppExtra::createCoreMenu(QWidget *parent) {
        auto menu = new CMenu(parent);
        menu->menuAction()->setMenuRole(QAction::NoRole);
        menu->setProperty("core-style", true);
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
        return QCoreApplication::translate("Application", "Explorer");
#elif defined(Q_OS_MAC)
        return QCoreApplication::translate("Application", "Finder");
#else
        return QCoreApplication::translate("Application", "File Manager");
#endif
    }

}