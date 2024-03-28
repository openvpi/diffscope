#ifndef AUDIO_VSTCONNECTIONSYSTEM_H
#define AUDIO_VSTCONNECTIONSYSTEM_H

#include <QObject>
#include <QPointer>

namespace talcs {
    class MixerAudioSource;
    class AudioSourcePlayback;
    class RemoteSocket;
    class RemoteAudioDevice;
    class RemoteEditor;
}

namespace Audio {

    class VSTAddOn;

    class VSTConnectionSystem : public QObject {
        Q_OBJECT
    public:
        explicit VSTConnectionSystem(QObject *parent = nullptr);
        ~VSTConnectionSystem() override;

        bool initialize();

        void setApplicationInitializing(bool);
        bool isApplicationInitializing() const;

        talcs::RemoteSocket *socket() const;
        talcs::RemoteAudioDevice *device() const;
        talcs::MixerAudioSource *preMixer() const;

        void testDevice();

        void setVSTAddOn(VSTAddOn *addOn);
        VSTAddOn *vstAddOn() const;

    Q_SIGNALS:
        void remoteSampleRateChanged(double sampleRate);
        void remoteBufferSizeChanged(qint64 bufferSize);
        void deviceRemoteOpenedHandled();

    private:
        talcs::RemoteSocket *m_socket = nullptr;
        QPointer<talcs::RemoteAudioDevice> m_dev;
        QPointer<talcs::RemoteEditor> m_editor;
        bool m_isApplicationInitializing = false;

        std::unique_ptr<talcs::AudioSourcePlayback> m_playback;
        talcs::MixerAudioSource *m_preMixer = nullptr;

        VSTAddOn *m_vstAddOn = nullptr;

        QByteArray getEditorData(bool *ok);
        bool setEditorData(const QByteArray &data);
        void handleRemoteDeviceRemoteOpened(qint64 bufferSize, double sampleRate, int maxChannelCount);
    };

} // Audio

#endif // AUDIO_VSTCONNECTIONSYSTEM_H
