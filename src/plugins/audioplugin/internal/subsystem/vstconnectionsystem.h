#ifndef AUDIO_VSTCONNECTIONSYSTEM_H
#define AUDIO_VSTCONNECTIONSYSTEM_H

#include <QPointer>

#include "abstractoutputsystem.h"

namespace talcs {
    class RemoteSocket;
    class RemoteAudioDevice;
    class RemoteEditor;
    class RemoteMidiMessageIntegrator;
    class MidiNoteSynthesizer;
}

namespace Audio::Internal {

    class ProjectAddOn;

    class AudioDeviceTesterAudioSource;

    class VSTConnectionSystem : public AbstractOutputSystem {
        Q_OBJECT
    public:
        explicit VSTConnectionSystem(QObject *parent = nullptr);
        ~VSTConnectionSystem() override;

        static bool createVSTConfig();

        bool initialize() override;

        void setApplicationInitializing(bool);
        bool isApplicationInitializing() const;

        talcs::RemoteSocket *socket() const;
        talcs::AudioDevice *device() const override;

        talcs::RemoteAudioDevice *remoteAudioDevice() const;
        talcs::RemoteEditor *remoteEditor() const;

        talcs::RemoteMidiMessageIntegrator *integrator() const;
        talcs::MidiNoteSynthesizer *synthesizer() const;

        void syncSynthesizerPreference();


        bool makeReady() override;

        void setVSTAddOn(ProjectAddOn *addOn);
        ProjectAddOn *vstAddOn() const;

        QPair<QString, QString> hostSpecs() const;

    signals:
        void hostSpecsChanged(const QString &hostExecutable, const QString &pluginFormat);

    private:
        talcs::RemoteSocket *m_socket = nullptr;
        QPointer<talcs::RemoteAudioDevice> m_dev;
        QPointer<talcs::RemoteEditor> m_editor;
        bool m_isApplicationInitializing = false;

        std::unique_ptr<talcs::RemoteMidiMessageIntegrator> m_integrator;
        talcs::MidiNoteSynthesizer *m_synthesizer;
        std::unique_ptr<talcs::MixerAudioSource> m_synthesizerMixer;

        ProjectAddOn *m_vstAddOn = nullptr;

        QPair<QString, QString> m_hostSpecs;

        QByteArray getEditorData(bool *ok);
        bool setEditorData(const QByteArray &data);
        void handleRemoteDeviceRemoteOpened(qint64 bufferSize, double sampleRate, int maxChannelCount);

        void setHostSpecs(const QString &hostExecutable, const QString &pluginFormat);
    };

}

#endif // AUDIO_VSTCONNECTIONSYSTEM_H
