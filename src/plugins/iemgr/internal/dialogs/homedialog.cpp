#include "homedialog.h"

#include <QDir>
#include <QFileDialog>

#include <QMCore/qmchronomap.h>
#include <QMCore/qmsystem.h>
#include <QMWidgets/clineedit.h>
#include <QMWidgets/ctabbutton.h>

#include <CoreApi/ILoader.h>

#include <coreplugin/ICore.h>

#include "wizardmanager.h"

namespace IEMgr::Internal {

    static constexpr const char KEY_NAME_IMPORT_DIALOG_BASE_PATH[] = "IEMgr/FileDialogPath";

    HomeDialog::HomeDialog(QWidget *parent) : SVS::WorkflowDialog(parent) {
        curWizard = nullptr;

        // Left
        leftLayout = new QVBoxLayout();
        leftLayout->setMargin(0);
        leftLayout->setSpacing(0);

        buttonGroup = new QButtonGroup(this);
        buttonGroup->setExclusive(true);

        initWizardsDisplay();

        auto leftWidget = new QFrame();
        leftWidget->setLayout(leftLayout);

        // Right
        pathLabel = new QLabel(tr("File Path:"));

        lineEdit = new CLineEdit();
        lineEdit->setClearButtonEnabled(true);

        browseButton = new QPushButton(tr("Browse"));

        textButtonLayout = new QHBoxLayout();
        textButtonLayout->setMargin(0);
        textButtonLayout->setSpacing(0);

        hintLabel = new QLabel();

        textButtonLayout->addWidget(pathLabel);
        textButtonLayout->addWidget(lineEdit);
        textButtonLayout->addWidget(browseButton);

        rightLayout = new QVBoxLayout();
        rightLayout->setMargin(0);
        rightLayout->setSpacing(0);
        rightLayout->addLayout(textButtonLayout);
        rightLayout->addWidget(hintLabel);
        rightLayout->addStretch();

        auto rightWidget = new QFrame();
        rightWidget->setLayout(rightLayout);

        // Page
        page = new SVS::WorkflowPage();
        page->setSideWidget(leftWidget);
        page->setWidget(rightWidget);
        page->setButtons(SVS::WorkflowPage::SingleStepButtons);
        page->setButtonEnabled(SVS::WorkflowPage::OkButton, false);
        setPage(page);

        connect(lineEdit, &QLineEdit::textChanged, this, &HomeDialog::_q_textChanged);
        connect(browseButton, &QAbstractButton::clicked, this, &HomeDialog::_q_browseButtonClicked);

        auto btn = buttonGroup->button(0);
        if (btn) {
            btn->click();
        }

        setWindowTitle(tr("Import Project"));

        resize(800, 500);
        page->splitter()->setSizes({200, 600});
    }

    HomeDialog::~HomeDialog() {
    }

    IWizard *HomeDialog::currentWizard() const {
        return curWizard;
    }

    QString HomeDialog::currentPath() const {
        return lineEdit->text();
    }

    void HomeDialog::setCurrentPath(const QString &path) {
        lineEdit->setText(path);
    }

    void HomeDialog::selectWizard(IEMgr::IWizard *wizard) {
        page->setTitle(wizard->displayName());
        page->setDescription(wizard->description());
        lineEdit->clear();

        curWizard = wizard;
    }

    bool HomeDialog::finish() {
        return true;
    }

    void HomeDialog::initWizardsDisplay() {
        auto wizards = WizardManager::instance()->wizards();

        struct Category {
            QString name;
            QList<QAbstractButton *> buttons;
        };

        QMChronoMap<QString, Category> categories;
        for (const auto &wizard : qAsConst(wizards)) {
            if (!(wizard->features() & IWizard::ImportProject)) {
                continue;
            }

            auto btn = new CTabButton(wizard->displayName());
            btn->setCheckable(true);
            btn->setToolTip(wizard->description());
            btn->setProperty("wizard", QVariant::fromValue(wizard));
            connect(btn, &QAbstractButton::clicked, this, &HomeDialog::_q_itemButtonClicked);

            auto it = categories.find(wizard->category());
            if (it == categories.end()) {
                categories.append(wizard->category(), {wizard->displayCategory(), {btn}});
            } else {
                auto &category = it.value();
                if (category.name.isEmpty()) {
                    category.name = wizard->displayCategory();
                }
                category.buttons << btn;
            }
        }

        int buttonId = 0;
        for (const auto &category : qAsConst(categories)) {
            auto label = new QLabel(category.name);
            label->setAlignment(Qt::AlignCenter);
            leftLayout->addWidget(label);

            for (const auto &btn : category.buttons) {
                leftLayout->addWidget(btn);
                buttonGroup->addButton(btn, buttonId++);
            }
        }

        leftLayout->addStretch();
    }

    void HomeDialog::_q_browseButtonClicked() {
        if (!curWizard) {
            return;
        }

        auto &settings = *Core::ILoader::instance()->settings();

        auto basePath = settings.value(KEY_NAME_IMPORT_DIALOG_BASE_PATH).toString();
        auto path = QFileDialog::getOpenFileName(this, tr("Browse"), basePath,
                                                 curWizard->filter(IWizard::ImportProject));
        if (path.isEmpty()) {
            return;
        }
        settings.insert(KEY_NAME_IMPORT_DIALOG_BASE_PATH, QM::PathFindDirPath(path));

        lineEdit->setText(QDir::toNativeSeparators(path));
    }

    void HomeDialog::_q_itemButtonClicked() {
        auto btn = qobject_cast<QAbstractButton *>(sender());
        auto wizard = btn->property("wizard").value<IWizard *>();

        if (!wizard) {
            return;
        }

        selectWizard(wizard);
    }

    void HomeDialog::_q_textChanged(const QString &text) {
        if (text.isEmpty()) {
            hintLabel->setText({});
            page->setButtonEnabled(SVS::WorkflowPage::OkButton, false);
            return;
        }

        if (!QM::isFileExist(text)) {
            hintLabel->setText(tr("File doesn't exist."));
            page->setButtonEnabled(SVS::WorkflowPage::OkButton, false);
        } else {
            hintLabel->setText({});
            page->setButtonEnabled(SVS::WorkflowPage::OkButton, true);
        }
    }

}