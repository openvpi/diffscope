#include "homewindow.h"

#include <QApplication>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmequalboxlayout.h>
#include <QMWidgets/qmdecoratorv2.h>

#include "icore.h"
#include "homerecentwidget.h"

namespace Core::Internal {

    HomeWindow::HomeWindow(QWidget *parent) {
        setAcceptDrops(true);

        // Initialize nav frame
        m_navFrame = new CNavFrame();
        m_navFrame->setObjectName(QStringLiteral("home-frame"));
        setCentralWidget(m_navFrame);

        m_titleButton = new CTabButton(qApp->applicationName());
        m_titleButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        m_navFrame->setTopWidget(m_titleButton);

        {
            m_settingsButton = new CTabButton();
            m_aboutButton = new CTabButton();

            m_bottomButtonsLayout = new QBoxLayout(QBoxLayout::BottomToTop);
            m_bottomButtonsLayout->setContentsMargins({});
            m_bottomButtonsLayout->setSpacing(0);
            m_bottomButtonsLayout->addWidget(m_aboutButton);
            m_bottomButtonsLayout->addWidget(m_settingsButton);

            m_bottomButtonsFrame = new QFrame();
            m_bottomButtonsFrame->setObjectName("bottom-buttons-widget");
            m_bottomButtonsFrame->setLayout(m_bottomButtonsLayout);

            m_navFrame->setBottomWidget(m_bottomButtonsFrame);

            connect(m_settingsButton, &QAbstractButton::clicked, this,
                    &HomeWindow::_q_settingsButtonClicked);
            connect(m_aboutButton, &QAbstractButton::clicked, this,
                    &HomeWindow::_q_aboutButtonClicked);
        }

        // Add recent widget
        m_recentWidget = new HomeRecentWidget();
        m_recentWidgetButton = m_navFrame->addWidget(m_recentWidget);

        auto recentTopWidget = m_recentWidget->topWidget;
        connect(recentTopWidget, &HomeRecentTopFrame::newRequested, this,
                &HomeWindow::_q_newButtonClicked);
        connect(recentTopWidget, &HomeRecentTopFrame::openRequested, this,
                &HomeWindow::_q_openButtonClicked);

        auto recentBottomWidget = m_recentWidget->bottomWidget;
        connect(recentBottomWidget, &HomeRecentBottomFrame::openFileRequested, this,
                &HomeWindow::_q_openFileRequested);

        qIDec->installLocale(this, _LOC(HomeWindow, this));
    }

    HomeWindow::~HomeWindow() {
    }

    void HomeWindow::reloadStrings() {
        m_recentWidgetButton->setText(tr("Recent"));
        m_settingsButton->setText(tr("Settings"));
        m_aboutButton->setText(tr("About %1").arg(qApp->applicationName()));
    }

    QLayout *HomeWindow::recentTopLayout() const {
        return m_recentWidget->topWidget->layout();
    }

    void HomeWindow::_q_newButtonClicked() {
        ICore::instance()->newFile();
    }

    void HomeWindow::_q_openButtonClicked() {
        ICore::instance()->openFile({}, this);
    }

    void HomeWindow::_q_settingsButtonClicked() {
        ICore::instance()->showSettingsDialog(QStringLiteral("core.Settings"), this);
    }

    void HomeWindow::_q_aboutButtonClicked() {
        ICore::aboutApp(this);
    }

    void HomeWindow::_q_openFileRequested(const QString &fileName) {
        ICore::instance()->openFile(fileName, this);
    }

}