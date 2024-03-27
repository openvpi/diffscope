#ifndef HOMEDIALOG_H
#define HOMEDIALOG_H

#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include <SVSCraftWidgets//workflowdialog.h>
#include <SVSCraftWidgets//workflowpage.h>

#include <iemgr/iwizard.h>

namespace IEMgr::Internal {

    class HomeDialog : public SVS::WorkflowDialog {
        Q_OBJECT
    public:
        explicit HomeDialog(QWidget *parent = nullptr);
        ~HomeDialog();

    public:
        IWizard *currentWizard() const;

        QString currentPath() const;
        void setCurrentPath(const QString &path);

        void selectWizard(IWizard *wizard);

    protected:
        bool finish() override;

    private:
        SVS::WorkflowPage *page;

        QVBoxLayout *leftLayout;
        QVBoxLayout *rightLayout;

        QHash<QString, QLabel *> labels;
        QButtonGroup *buttonGroup;

        QHBoxLayout *textButtonLayout;
        QLineEdit *lineEdit;
        QPushButton *browseButton;
        QLabel *pathLabel;
        QLabel *hintLabel;

        IWizard *curWizard;

        void initWizardsDisplay();

    private:
        void _q_browseButtonClicked();
        void _q_itemButtonClicked();
        void _q_textChanged(const QString &text);
    };

}

#endif // HOMEDIALOG_H