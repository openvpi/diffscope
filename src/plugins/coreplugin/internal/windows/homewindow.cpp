#include "homewindow.h"

#include <QApplication>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmdecoratorv2.h>

#include "icore.h"
#include "homerecentwidget.h"

namespace Core::Internal {

    HomeWindow::HomeWindow(QWidget *parent) {
        setAcceptDrops(true);

        // Initialize nav frame
        navFrame = new CNavFrame();
        navFrame->setObjectName(QStringLiteral("home-frame"));
        setCentralWidget(navFrame);

        titleButton = new CTabButton(qApp->applicationName());
        titleButton->setObjectName(QStringLiteral("home-title-button"));
        titleButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        navFrame->setTopWidget(titleButton);

        aboutButton = new CTabButton();
        aboutButton->setProperty("type", QStringLiteral("home-bottom-button"));
        aboutButton->setObjectName(QStringLiteral("home-about-button"));
        navFrame->setBottomWidget(aboutButton);

        connect(aboutButton, &QAbstractButton::clicked, this, &HomeWindow::_q_aboutButtonClicked);

        // Add recent widget
        auto recentWidget = new HomeRecentWidget();
        recentWidgetButton = navFrame->addWidget(recentWidget);

        auto recentTopWidget = recentWidget->topWidget;
        connect(recentTopWidget, &HomeRecentTopFrame::newRequested, this,
                &HomeWindow::_q_newButtonClicked);
        connect(recentTopWidget, &HomeRecentTopFrame::openRequested, this,
                &HomeWindow::_q_openButtonClicked);

        auto recentBottomWidget = recentWidget->bottomWidget;
        connect(recentBottomWidget, &HomeRecentBottomFrame::openFileRequested, this,
                &HomeWindow::_q_openFileRequested);

        qIDec->installLocale(this, _LOC(HomeWindow, this));
    }

    HomeWindow::~HomeWindow() {
    }

    void HomeWindow::reloadStrings() {
        recentWidgetButton->setText(tr("Recent"));
        aboutButton->setText(tr("About %1").arg(qApp->applicationName()));
    }

    void HomeWindow::_q_newButtonClicked() {
        ICore::instance()->newFile();
    }

    void HomeWindow::_q_openButtonClicked() {
        ICore::instance()->openFile({}, this);
    }

    void HomeWindow::_q_aboutButtonClicked() {
        ICore::aboutApp(this);
    }

    void HomeWindow::_q_openFileRequested(const QString &fileName) {
        ICore::instance()->openFile(fileName, this);
    }

}