#include "icore.h"

#include <csignal>

#include "QStyleFactory"
#include <QApplication>
#include <QChildEvent>
#include <QMessageBox>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QGridLayout>

#include <extensionsystem/pluginmanager.h>

#include <QMCore/qmsystem.h>

#include <CoreApi/iloader.h>
#include <CoreApi/private/icorebase_p.h>

#include <choruskit_config.h>
#include <choruskit_buildinfo.h>

namespace Core {

    class ICorePrivate : ICoreBasePrivate {
        Q_DECLARE_PUBLIC(ICore)
    public:
        ICorePrivate() {
        }

        void init() {
        }
    };

    ICore *ICore::instance() {
        return qobject_cast<ICore *>(ICoreBase::instance());
    }

    QString ICore::mainTitle() {
        return QString("%1 %2").arg(QApplication::organizationName(),
                                    QApplication::applicationName());
    }

    QString ICore::displayTitle(const QString &text) {
        return QString("%1 - %2").arg(text, mainTitle());
    }

    void ICore::aboutApp(QWidget *parent) {
        QString copyrightInfo =
            tr("<p>Based on Qt version %1.<br>"
               "Copyright 2019-%2 Team OpenVPI. All rights reserved.</p>")
                .arg(QStringLiteral(QT_VERSION_STR), QStringLiteral(DIFFSCOPE_BUILD_YEAR));

        QString buildInfo = tr("<h3>Build Information</h3>"
                               "<p>"
                               "Version: %1<br>"
                               "Branch: %2<br>"
                               "Commit: %3<br>"
                               "Build date: %4<br>"
                               "Toolchain: %5 %6 %7"
                               "</p>")
                                .arg(QApplication::applicationVersion(),
                                     QStringLiteral(DIFFSCOPE_GIT_BRANCH),           //
                                     QStringLiteral(DIFFSCOPE_GIT_LAST_COMMIT_HASH), //
                                     QStringLiteral(DIFFSCOPE_BUILD_TIME),           //
                                     QStringLiteral(DIFFSCOPE_COMPILER_ARCH),        //
                                     QStringLiteral(DIFFSCOPE_COMPILER_ID),          //
                                     QStringLiteral(DIFFSCOPE_COMPILER_VERSION));

        QString aboutInfo =
            tr("<h3>About</h3>"
               "<p>DiffScope is a kind of implementation of DiffSinger graphical editing tool, "
               "included in ChorusKit toolset.</p>");

        QString licenseInfo =
            tr("<h3>License</h3>"
               "<p>Licensed under the Apache License, Version 2.0.<br>"
               R"(You may obtain a copy of the License at %1.</p>)"
               "<p>This application is distributed "
               "<b>AS IS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND</b>, either express or "
               "implied.</p>")
                .arg(
                    "<a "
                    "href=\"https://www.apache.org/licenses/LICENSE-2.0\">apache.org/licenses</a>");

        QString translatedTextAboutQtCaption =
            tr("<h2>ChorusKit DiffScope</h2>%1%2%3%4")
                .arg(copyrightInfo, buildInfo, aboutInfo, licenseInfo);

        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(tr("About %1").arg(qApp->applicationName()));
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

    // int ICore::showSettingsDialog(const QString &id, QWidget *parent) {
    //     static Internal::SettingsDialog *dlg = nullptr;

    //     if (dlg) {
    //         dlg->selectPage(id);
    //         return -1;
    //     }

    //     int code;
    //     {
    //         Internal::SettingsDialog dlg2(parent);
    //         dlg = &dlg2;
    //         dlg2.selectPage(id);
    //         code = dlg2.exec();
    //         dlg = nullptr;
    //     }

    //     return code;
    // }

    // void ICore::showHome() {
    //     auto inst = IHomeWindow::instance();
    //     if (inst) {
    //         QMView::bringWindowToForeground(inst->window());
    //         return;
    //     }
    //     IWindow::create<IHomeWindow>();
    // }

    void ICore::fatalError(QWidget *parent, const QString &text, int exitCode) {
        QMessageBox msgBox(QMessageBox::Critical, tr("Fatal Error"), text, QMessageBox::Ok, parent);
        msgBox.setText(tr("%1\n\nThe application is about to exit because an unresolvable "
                          "exception has occurred, "
                          "which may be caused by an illegal operation of an plugin.")
                           .arg(text));
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        std::exit(exitCode);
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : ICoreBase(d, parent) {
        d.init();
    }


}