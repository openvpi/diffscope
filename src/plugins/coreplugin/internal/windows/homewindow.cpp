#include "homewindow.h"

#include <QApplication>
#include <QMessageBox>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmequalboxlayout.h>
#include <QMWidgets/qmdecoratorv2.h>

#include "icore.h"
#include "appextra.h"
#include "homerecentwidget.h"

namespace Core::Internal {

    class NavFrame : public CNavFrame {
    public:
        explicit NavFrame(QWidget *parent = nullptr) : CNavFrame(parent) {
        }

    protected:
        void widgetAdded(QWidget *w, QAbstractButton *btn) override {
            Q_UNUSED(w)
            btn->setProperty("type", QStringLiteral("home-nav-button"));
        }
    };

    HomeWindow::HomeWindow(QWidget *parent) {
        setAcceptDrops(true);

        // Initialize nav frame
        m_navFrame = new NavFrame();
        m_navFrame->setObjectName(QStringLiteral("home-frame"));
        setCentralWidget(m_navFrame);

        m_titleButton = new CTabButton(qApp->applicationName());
        m_titleButton->setObjectName(QStringLiteral("home-title-button"));
        m_titleButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        m_navFrame->setTopWidget(m_titleButton);

        {
            m_configureButton = new CTabButton();
            m_helpButton = new CTabButton();

            m_bottomButtonsLayout = new QBoxLayout(QBoxLayout::BottomToTop);
            m_bottomButtonsLayout->setContentsMargins({});
            m_bottomButtonsLayout->setSpacing(0);
            m_bottomButtonsLayout->addWidget(m_helpButton);
            m_bottomButtonsLayout->addWidget(m_configureButton);

            m_bottomButtonsFrame = new QFrame();
            m_bottomButtonsFrame->setObjectName("home-bottom-buttons-frame");
            m_bottomButtonsFrame->setLayout(m_bottomButtonsLayout);

            m_navFrame->setBottomWidget(m_bottomButtonsFrame);

            connect(m_configureButton, &QAbstractButton::clicked, this,
                    &HomeWindow::_q_configureButtonClicked);
            connect(m_helpButton, &QAbstractButton::clicked, this,
                    &HomeWindow::_q_aboutButtonClicked);
        }

        // Add recent widget
        m_recentWidget = new HomeRecentWidget();
        m_recentWidgetButton = m_navFrame->addWidget(m_recentWidget);

        auto recentBottomWidget = m_recentWidget->bottomWidget;
        connect(recentBottomWidget, &HomeRecentBottomFrame::openFileRequested, this,
                &HomeWindow::_q_openFileRequested);

        qIDec->installLocale(this, _LOC(HomeWindow, this));
        // qIDec->installTheme(this, "core.HomeWindow");
    }

    HomeWindow::~HomeWindow() {
    }

    void HomeWindow::reloadStrings() {
        m_recentWidgetButton->setText(tr("Recent"));
        m_configureButton->setText(tr("Configure"));
        m_helpButton->setText(tr("Help"));
    }

    void HomeWindow::addFileButton(QAbstractButton *button) {
        m_recentWidget->topWidget->addButton(button);
    }

    void HomeWindow::removeFileButton(QAbstractButton *button) {
        m_recentWidget->topWidget->removeButton(button);
    }

    void HomeWindow::_q_newButtonClicked() {
        ICore::instance()->newFile();
    }

    void HomeWindow::_q_openButtonClicked() {
        ICore::instance()->openFile({}, this);
    }

    void HomeWindow::_q_configureButtonClicked() {
        auto menu = AppExtra::createCoreMenu(this);
        auto pluginsAction = menu->addAction(tr("Plugins"));
        auto settingsAction = menu->addAction(tr("Settings"));
        auto action = menu->exec(QCursor::pos());
        if (action == pluginsAction) {
            ICore::showPluginsDialog(this);
        } else if (action == settingsAction) {
            ICore::showSettingsDialog(QStringLiteral("core.Settings"), this);
        }
        menu->deleteLater();
    }

    void HomeWindow::_q_aboutButtonClicked() {
        auto menu = AppExtra::createCoreMenu(this);
        auto aboutAppAction = menu->addAction(tr("About %1").arg(qApp->applicationName()));
        auto aboutQtAction = menu->addAction(tr("About Qt"));
        auto action = menu->exec(QCursor::pos());
        if (action == aboutAppAction) {
            AppExtra::aboutApp(this);
        } else if (action == aboutQtAction) {
            QMessageBox::aboutQt(this, tr("About Qt"));
        }
        menu->deleteLater();
    }

    void HomeWindow::_q_openFileRequested(const QString &fileName) {
        ICore::instance()->openFile(fileName, this);
    }

}