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

// Analyzers
#include "mandarinanalyzer.h"
#include "cantoneseanalyzer.h"
#include "englishanalyzer.h"
#include "pinyinanalyzer.h"
#include "romajianalyzer.h"
#include "kanaanalyzer.h"

#include "linebreakanalyzer.h"
#include "numberanalyzer.h"
#include "punctuationanalyzer.h"
#include "sluranalyzer.h"
#include "spaceanalyzer.h"
#include "unknownanalyzer.h"

// G2ps
#include "mandarinconverter.h"
#include "cantoneseconverter.h"
#include "englishconverter.h"
#include "kanaconverter.h"
#include "unknownconverter.h"

#include "languagemanager.h"

CK_STATIC_ACTION_EXTENSION_GETTER(lyrictool_actions, getMyActionExtension);

namespace LyricTool::Internal {

    static LanguageManager *langMgr = nullptr;

    LyricToolPlugin::LyricToolPlugin() {
    }

    LyricToolPlugin::~LyricToolPlugin() {
    }

    bool LyricToolPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));
        qIDec->addThemePath(pluginSpec()->location() + QStringLiteral("/themes"));

        const auto splash = Core::InitRoutine::splash();
        splash->showMessage(tr("Initializing lyric manager..."));

        // Init LanguageManager instance
        langMgr = new LanguageManager(this);

        const auto icore = Core::ICore::instance();

        // Add basic actions
        const auto domain = icore->actionManager()->domain();
        domain->addExtension(getMyActionExtension());

        // Add basic windows and add-ons
        // Core::IHomeWindowRegistry::instance()->attach<HomeAddOn>();
        // Core::IProjectWindowRegistry::instance()->attach<ProjectAddOn>();

        // // Add wizards
        langMgr->addLanguage(new MandarinAnalyzer());
        langMgr->addLanguage(new CantoneseAnalyzer);
        langMgr->addLanguage(new EnglishAnalyzer());
        langMgr->addLanguage(new PinyinAnalyzer());
        langMgr->addLanguage(new RomajiAnalyzer());
        langMgr->addLanguage(new KanaAnalyzer());

        langMgr->addLanguage(new NumberAnalyzer());
        langMgr->addLanguage(new LinebreakAnalyzer());
        langMgr->addLanguage(new PunctuationAnalyzer());
        langMgr->addLanguage(new SlurAnalyzer());
        langMgr->addLanguage(new SpaceAnalyzer());
        langMgr->addLanguage(new UnknownAnalyzer());

        langMgr->addG2p(new MandarinConverter());
        langMgr->addG2p(new CantoneseConverter());
        langMgr->addG2p(new EnglishConverter());
        langMgr->addG2p(new KanaConverter());
        langMgr->addG2p(new UnknownConverter());

        return true;
    }

    void LyricToolPlugin::extensionsInitialized() {
    }

    bool LyricToolPlugin::delayedInitialize() {
        return false;
    }

}