#include "audioplugin.h"

#include <QMessageBox>

#include <extensionsystem/pluginspec.h>

#include <QMWidgets/qmdecoratorv2.h>

#include <appshared/initroutine.h>

#include <icore.h>

#include "audiosystem.h"
#include "outputsystem.h"
#include "vstconnectionsystem.h"
#include "audiopage.h"
#include "outputplaybackpage.h"
#include "vstmodepage.h"

namespace Audio {

    AudioPlugin::AudioPlugin() {
    }

    AudioPlugin::~AudioPlugin() = default;

    bool AudioPlugin::initialize(const QStringList &arguments, QString *errorString) {
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));
        auto ir = AppShared::InitRoutine::instance();
        qDebug() << "Audio::AudioPlugin: initializing";
        ir->splash->showMessage(tr("Initializing audio plugin..."));

        auto settings = Core::ILoader::instance()->settings();
        if (!settings->contains("Audio"))
            settings->insert("Audio", QJsonObject());

        new AudioSystem(this);

        if (arguments.contains("-vst")) {
            qDebug() << "Audio::AudioPlugin: started by an external host (primary instance)";
            AudioSystem::vstConnectionSystem()->setApplicationInitializing(true);
        }

        if (!AudioSystem::outputSystem()->initialize()) {
            QMessageBox msgBox(ir->splash);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Cannot initialize audio output system"));
            msgBox.setInformativeText(tr("%1 will not play any sound because no available audio output device found. Please check the status of the audio driver and device.").arg(QApplication::applicationName()));
            msgBox.exec();
        }
        if (!AudioSystem::vstConnectionSystem()->initialize()) {
            QMessageBox msgBox(ir->splash);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Cannot initialize Plugin Mode connection system"));
            msgBox.setInformativeText(tr("%1 will not be able to establish a connection with %1 Bridge. Please check the Plugin Mode configuration in Settings.").arg(QApplication::applicationName()));
            msgBox.exec();
        }

        auto sc = Core::ICore::instance()->settingCatalog();
        auto audioPage = new AudioPage;
        audioPage->addPage(new OutputPlaybackPage);
        audioPage->addPage(new VSTModePage);
        sc->addPage(audioPage);
        return true;
    }
    void AudioPlugin::extensionsInitialized() {
        if (AudioSystem::vstConnectionSystem()->isApplicationInitializing()) {

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

} // Audio