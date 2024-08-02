#include "audioplugin.h"

#include <QMessageBox>
#include <QApplication>

#include <TalcsFormat/FormatManager.h>

#include <extensionsystem/pluginspec.h>

#include <QMWidgets/qmdecoratorv2.h>

#include <CoreApi/iloader.h>

#include <idecoreFramework/initroutine.h>

#include <coreplugin/icore.h>
#include <coreplugin/iprojectwindow.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>
#include <audioplugin/internal/audiopage.h>
#include <audioplugin/internal/outputplaybackpage.h>
#include <audioplugin/internal/vstmodepage.h>
#include <audioplugin/iaudio.h>
#include <audioplugin/private/iaudio_p.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/outputsysteminterface_p.h>
#include <audioplugin/internal/devicetester.h>
#include <audioplugin/internal/projectaddon.h>
#include <audioplugin/internal/builtinformatentry.h>
#include <audioplugin/internal/midipage.h>
#include <audioplugin/internal/midisystem.h>
#include <audioplugin/internal/audioexportdialog.h>

namespace Audio::Internal {

    AudioPlugin::AudioPlugin() {
    }

    AudioPlugin::~AudioPlugin() = default;

    bool AudioPlugin::initialize(const QStringList &arguments, QString *errorString) {
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));

        qDebug() << "Audio::AudioPlugin: initializing";
        auto splash = Core::InitRoutine::splash();
        splash->showMessage(tr("Initializing audio plugin..."));

        auto settings = Core::ILoader::instance()->settings();
        if (!settings->contains("Audio"))
            settings->insert("Audio", QJsonObject());

        auto sc = Core::ICore::instance()->settingCatalog();
        auto audioPage = new AudioPage;
        audioPage->addPage(new OutputPlaybackPage);
        audioPage->addPage(new VSTModePage);
        audioPage->addPage(new MIDIPage);
        sc->addPage(audioPage);

        Core::IProjectWindowRegistry::instance()->attach<ProjectAddOn>();

        new AudioSystem(this);

        auto iAudio = new IAudio;
        iAudio->d_func()->outputSystemInterface = new OutputSystemInterface(AudioSystem::outputSystem());
        iAudio->d_func()->outputSystemInterface->d_func()->init(AudioSystem::outputSystem(), false);
        iAudio->d_func()->vstOutputSystemInterface = new OutputSystemInterface(AudioSystem::vstConnectionSystem());
        iAudio->d_func()->vstOutputSystemInterface->d_func()->init(AudioSystem::vstConnectionSystem(), true);
        iAudio->d_func()->formatManager = new talcs::FormatManager(this);

        iAudio->formatManager()->addEntry(new BuiltInFormatEntry);

        if (arguments.contains("-vst")) {
            qDebug() << "Audio::AudioPlugin: started by an external host (primary instance)";
            AudioSystem::vstConnectionSystem()->setApplicationInitializing(true);
        }

        if (!AudioSystem::outputSystem()->initialize()) {
            QMessageBox msgBox(splash);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Cannot initialize audio output system"));
            msgBox.setInformativeText(tr("%1 will not play any sound because no available audio output device found. Please check the status of the audio driver and device.").arg(QApplication::applicationName()));
            msgBox.exec();
        }
        if (!AudioSystem::vstConnectionSystem()->initialize()) {
            QMessageBox msgBox(splash);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Cannot initialize Plugin Mode connection system"));
            msgBox.setInformativeText(tr("%1 will not be able to establish a connection with %1 Bridge. Please check the Plugin Mode configuration in Settings.").arg(QApplication::applicationName()));
            msgBox.exec();
        }
        if (!AudioSystem::midiSystem()->initialize()) {

        }

        new DeviceTester(iAudio->outputSystemInterface(true), iAudio->outputSystemInterface(true));
        new DeviceTester(iAudio->outputSystemInterface(false), iAudio->outputSystemInterface(false));

        AudioExportDialog dlg;
        dlg.exec();

        return true;
    }
    void AudioPlugin::extensionsInitialized() {
        if (AudioSystem::vstConnectionSystem()->isApplicationInitializing()) {
            // TODO open VST window
        }

    }
    bool AudioPlugin::delayedInitialize() {
        return false;
    }
    QObject *AudioPlugin::remoteCommand(const QStringList &options, const QString &workingDirectory, const QStringList &args) {
        if (options.contains("-vst")) {
            qDebug() << "Audio::AudioPlugin: started by an external host (secondary instance)";
        } else {

        }
        return nullptr;
    }

}