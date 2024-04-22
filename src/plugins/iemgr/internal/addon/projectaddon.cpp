#include "projectaddon.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <coreplugin/iprojectwindow.h>
#include <coreplugin/appextra.h>

#include "wizardmanager.h"

namespace IEMgr::Internal {

    ProjectAddOn::ProjectAddOn(QObject *parent) : IWindowAddOn(parent) {
    }

    ProjectAddOn::~ProjectAddOn() {
    }
    void ProjectAddOn::initialize() {
        initActions();
    }

    void ProjectAddOn::extensionsInitialized() {
    }

    void ProjectAddOn::reloadStrings() {
    }

    void ProjectAddOn::initActions() {
        auto iWin = windowHandle()->cast<Core::IProjectWindow>();
        auto win = iWin->window();

        // Actions
        ai.ImportProject = Core::AppExtra::actionItem(QStringLiteral("ImportProject"), this);
        ai.AppendProject = Core::AppExtra::actionItem(QStringLiteral("AppendProject"), this);
        ai.ExportProject = Core::AppExtra::actionItem(QStringLiteral("ExportProject"), this);

        iWin->addActionItems({
            ai.ImportProject,
            ai.AppendProject,
            ai.ExportProject,
        });

        // Signals and slots
        connect(ai.ImportProject->action(), &QAction::triggered, this, [win]() {
            WizardManager::instance()->runImport(win); //
        });
    }

}