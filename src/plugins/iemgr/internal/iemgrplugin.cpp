#include "iemgrplugin.h"

#include <QApplication>
#include <QThread>

#include <QMCore/qmsystem.h>
#include <QMWidgets/qmdecoratorv2.h>

#include <extensionsystem/pluginspec.h>

#include <CoreApi/iloader.h>

#include <appshared/initroutine.h>

#include <coreplugin/icore.h>
#include <coreplugin/iprojectwindow.h>

#include "wizardmanager.h"
#include "homeaddon.h"

#include "midiwizard.h"
#include "opensvipwizard.h"
#include "svipwizard.h"
#include "ustwizard.h"

static inline auto getIEMgrActionExtension() {
    return CK_STATIC_ACTION_EXTENSION(iemgr_actions);
};

namespace IEMgr::Internal {

    static WizardManager *imgr = nullptr;

    IEMgrPlugin::IEMgrPlugin() {
    }

    IEMgrPlugin::~IEMgrPlugin() {
    }

    bool IEMgrPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
        qIDec->addThemePath(pluginSpec()->location() + "/themes");

        auto ir = AppShared::InitRoutine::instance();
        ir->splash->showMessage(tr("Initializing import/export manager..."));

        // Init WizardManager instance
        imgr = new WizardManager(this);

        auto icore = Core::ICore::instance();

        // Add basic actions
        Core::IProjectWindow::actionDomain()->addExtension(getIEMgrActionExtension());

        // Add basic windows and add-ons
        auto winMgr = icore->windowSystem();
        winMgr->addAddOn<HomeAddOn>(QStringLiteral("home"));

        // Add wizards
        imgr->addWizard(new MidiWizard());
        imgr->addWizard(new UstWizard());
        imgr->addWizard(new OpenSvipWizard());
        imgr->addWizard(new SvipWizard());

        return true;
    }

    void IEMgrPlugin::extensionsInitialized() {
    }

    bool IEMgrPlugin::delayedInitialize() {
        return false;
    }

    // This scope is only to expose strings to Qt translation tool
    static void _qt_translation_CommandCategory() {
        QApplication::translate("Core::CommandCategory", "Import");
        QApplication::translate("Core::CommandCategory", "Export");
    }

}
