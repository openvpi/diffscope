#ifndef AUDIO_VSTCONNECTIONSYSTEM_H
#define AUDIO_VSTCONNECTIONSYSTEM_H

#include <QPointer>

#include "abstractoutputsystem.h"

namespace talcs {
    class RemoteSocket;
    class RemoteAudioDevice;
    class RemoteEditor;
}

namespace Audio {

    class VSTAddOn;

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

        bool makeReady() override;

        void setVSTAddOn(VSTAddOn *addOn);
        VSTAddOn *vstAddOn() const;

        QPair<QString, QString> hostSpecs() const;

    signals:
        void hostSpecsChanged(const QString &hostExecutable, const QString &pluginFormat);

    private:
        talcs::RemoteSocket *m_socket = nullptr;
        QPointer<talcs::RemoteAudioDevice> m_dev;
        QPointer<talcs::RemoteEditor> m_editor;
        bool m_isApplicationInitializing = false;

        VSTAddOn *m_vstAddOn = nullptr;

        QPair<QString, QString> m_hostSpecs;

        QByteArray getEditorData(bool *ok);
        bool setEditorData(const QByteArray &data);
        void handleRemoteDeviceRemoteOpened(qint64 bufferSize, double sampleRate, int maxChannelCount);

        void setHostSpecs(const QString &hostExecutable, const QString &pluginFormat);
    };

} // Audio

#endif // AUDIO_VSTCONNECTIONSYSTEM_H
