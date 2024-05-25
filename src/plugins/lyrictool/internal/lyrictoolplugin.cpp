#include "lyrictoolplugin.h"

#include <QApplication>
#include <QThread>
#include <QMessageBox>

#include <QMCore/qmsystem.h>
#include <QMWidgets/qmdecoratorv2.h>

#include <extensionsystem/pluginspec.h>

#include <coreplugin/icore.h>
#include <coreplugin/initroutine.h>
#include <coreplugin/ihomewindow.h>
#include <coreplugin/iprojectwindow.h>

CK_STATIC_ACTION_EXTENSION_GETTER(lyrictool_actions, getMyActionExtension);

namespace LyricTool::Internal {

    // static WizardManager *imgr = nullptr;

    LyricToolPlugin::LyricToolPlugin() {
    }

    LyricToolPlugin::~LyricToolPlugin() {
    }

    bool LyricToolPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));
        qIDec->addThemePath(pluginSpec()->location() + QStringLiteral("/themes"));

        auto splash = Core::InitRoutine::splash();
        splash->showMessage(tr("Initializing lyric manager..."));

        // Init WizardManager instance
        // imgr = new WizardManager(this);

        auto icore = Core::ICore::instance();

        // Add basic actions
        auto domain = icore->actionManager()->domain();
        domain->addExtension(getMyActionExtension());

        // Add basic windows and add-ons
        // Core::IHomeWindowRegistry::instance()->attach<HomeAddOn>();
        // Core::IProjectWindowRegistry::instance()->attach<ProjectAddOn>();

        // // Add wizards
        // imgr->addWizard(new MidiWizard());
        // imgr->addWizard(new UstWizard());
        // imgr->addWizard(new OpenSvipWizard());
        // imgr->addWizard(new SvipWizard());

        return true;
    }

    void LyricToolPlugin::extensionsInitialized() {
    }

    bool LyricToolPlugin::delayedInitialize() {
        return false;
    }

}