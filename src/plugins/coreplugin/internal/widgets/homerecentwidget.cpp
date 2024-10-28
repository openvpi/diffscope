#include "homerecentwidget.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>

#include <SVSCraftWidgets/svscraftwidgetsnamespace.h>

#include <QMCore/qmsystem.h>
#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/clineedit.h>
#include <QMWidgets/cmenu.h>
#include <QMWidgets/qmsvgx.h>

#include "icore.h"
#include "appextra.h"

namespace Core::Internal {

    /**
     * @brief Recent widget top frame
     */
    HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : CBasicFrame(parent) {
        searchBox = new CLineEdit();
        searchBox->setClearButtonEnabled(true);
        searchBox->setObjectName("search-box");

        AppExtra::autoPolishPopupMenu(searchBox);

        topLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(0);
        topLayout->addWidget(searchBox);
        setLayout(topLayout);
        connect(searchBox, &QLineEdit::textChanged, this, &HomeRecentTopFrame::textChanged);

        qIDec->installLocale(this);
    }

    HomeRecentTopFrame::~HomeRecentTopFrame() {
    }

    void HomeRecentTopFrame::reloadStrings() {
        searchBox->setPlaceholderText(tr("Search for files"));
    }

    void HomeRecentTopFrame::addButton(QAbstractButton *btn) {
        topLayout->addWidget2(btn);
    }

    void HomeRecentTopFrame::removeButton(QAbstractButton *btn) {
        topLayout->removeWidget(btn);
    }

    /**
     * @brief Recent widget bottom frame
     */

    HomeRecentBottomFrame::HomeRecentBottomFrame(QWidget *parent) : CBasicFrame(parent) {
        fileWidget = new SVS::TitleListWidget();
        AppExtra::autoPolishScrollArea(fileWidget);

        fileWidget->setObjectName("home-file-widget");
        fileWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        emptyLabel = new QLabel();
        emptyLabel->setObjectName("empty-label");

        bottomLayout = new QVBoxLayout();
        bottomLayout->setContentsMargins(0, 0, 0, 0);
        bottomLayout->setSpacing(0);

        bottomLayout->addWidget(fileWidget);
        bottomLayout->addWidget(emptyLabel);
        bottomLayout->addStretch();

        bottomLayout->setStretchFactor(fileWidget, 1);

        setLayout(bottomLayout);

        auto docMgr = ICore::instance()->documentSystem();
        connect(docMgr, &DocumentSystem::recentFilesChanged, this,
                &HomeRecentBottomFrame::_q_recentFilesChanged);
        connect(fileWidget, &SVS::TitleListWidget::itemClickedEx, this,
                &HomeRecentBottomFrame::_q_itemClickedEx);

        // Reload recent files once
        reloadRecentFiles();

        qIDec->installLocale(this);
    }

    HomeRecentBottomFrame::~HomeRecentBottomFrame() {
    }

    void HomeRecentBottomFrame::reloadStrings() {
        updateEmptyLabel();
    }

    void HomeRecentBottomFrame::reloadRecentFiles() {
        auto docMgr = ICore::instance()->documentSystem();

        fileWidget->clear();

        int cnt = 0;
        for (const auto &fileName : docMgr->recentFiles()) {
            QFileInfo info(fileName);
            auto spec = docMgr->supportedDocType(info.completeSuffix());

            auto item = new QListWidgetItem();
            item->setData(SVS::DecorationRole, spec ? spec->icon() : QIcon());
            item->setData(SVS::DisplayRole, QDir::toNativeSeparators(info.absoluteFilePath()));
            item->setData(SVS::SubtitleRole, QDir::toNativeSeparators(info.absolutePath()));
            item->setData(SVS::DescriptionRole,
                          info.lastModified().toString(QStringLiteral("yyyy-MM-dd hh:mm")));

            fileWidget->addItem(item);

            if (++cnt > 20) {
                break;
            }
        }
        // fileWidget->resize(fileWidget->width(), fileWidget->contentsSize().height());
        updateListFilter();
    }

    void HomeRecentBottomFrame::setFilterKeyword(const QString &keyword) {
        m_keyword = keyword;
        m_keyword.replace("\\", "/");
        updateListFilter();
    }

    void HomeRecentBottomFrame::updateListFilter() {
        int cnt = 0;
        for (int i = 0; i < fileWidget->count(); ++i) {
            auto item = fileWidget->item(i);

            QString filename = item->data(Qt::DisplayRole).toString();
            bool visible = m_keyword.isEmpty() || filename.contains(m_keyword, Qt::CaseInsensitive);
            item->setHidden(!visible);

            if (visible) {
                cnt++;
            }
        }

        if (fileWidget->count() == 0 || cnt == 0) {
            fileWidget->hide();
            emptyLabel->show();
            updateEmptyLabel();
        } else {
            fileWidget->show();
            emptyLabel->hide();
        }
    }

    void HomeRecentBottomFrame::updateEmptyLabel() {
        emptyLabel->setText(fileWidget->count() == 0 ? tr("No data.") : tr("Nothing to show."));
    }

    void HomeRecentBottomFrame::_q_recentFilesChanged() {
        reloadRecentFiles();
    }

    void HomeRecentBottomFrame::_q_itemClickedEx(const QModelIndex &index, int button) {
        int type = QDir::Files;
        QString filename = index.data(Qt::DisplayRole).toString();
        if (button == Qt::LeftButton) {
            if (type == QDir::Files) {
                Q_EMIT openFileRequested(filename);
            } else {
                //
            }
        } else if (button == Qt::RightButton) {
            auto &menu = *AppExtra::createCoreMenu(this);

            QAction openAction(tr("Open(&O)"));
            // QAction newWinAction(tr("Open in new window(&E)"));
            QAction removeAction(tr("Remove from list(&R)"));
            QAction revealAction;

            menu.addAction(&openAction);
            // menu.addAction(&newWinAction);
            menu.addAction(&removeAction);

            if (type == QDir::Files) {
                revealAction.setText(tr("Show in %1(&S)").arg(AppExtra::fileManagerName()));
                menu.addSeparator();
                menu.addAction(&revealAction);
            } else if (type == QDir::Dirs) {
                revealAction.setText(tr("Open in %1(&S)").arg(AppExtra::fileManagerName()));
                menu.addSeparator();
                menu.addAction(&revealAction);
            }

            QAction *action = menu.exec(QCursor::pos());
            if (action == &openAction) {
                if (type == QDir::Files) {
                    Q_EMIT openFileRequested(filename);
                } else {
                    //
                }
            } else if (action == &removeAction) {
                ICore::instance()->documentSystem()->removeRecentFile(filename);
            } else if (action == &revealAction) {
                QM::reveal(filename);
            }

            delete &menu;
        }
    }

    /**
     * @brief Recent widget
     */
    HomeRecentWidget::HomeRecentWidget(QWidget *parent) : QSplitter(Qt::Vertical, parent) {
        topWidget = new HomeRecentTopFrame();
        topWidget->setObjectName("top-widget");
        topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        bottomWidget = new HomeRecentBottomFrame();
        bottomWidget->setObjectName("home-recent-list-container");
        bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        setChildrenCollapsible(false);

        addWidget(topWidget);
        addWidget(bottomWidget);

        setStretchFactor(0, 0);
        setStretchFactor(1, 1);

        qIDec->installLocale(this);

        connect(topWidget, &HomeRecentTopFrame::textChanged, this,
                &HomeRecentWidget::_q_searchTextChanged);
    }

    HomeRecentWidget::~HomeRecentWidget() {
    }

    void HomeRecentWidget::reloadStrings() {
    }

    void HomeRecentWidget::_q_searchTextChanged(const QString &text) {
        bottomWidget->setFilterKeyword(text);
    }

}