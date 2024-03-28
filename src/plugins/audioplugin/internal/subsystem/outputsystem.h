#ifndef AUDIO_OUTPUTSYSTEM_H
#define AUDIO_OUTPUTSYSTEM_H

#include <memory>

#include <QObject>

namespace talcs {
    class AudioDevice;
    class AudioDriver;
    class AudioDriverManager;
    class AudioSourcePlayback;
    class MixerAudioSource;
}

namespace Audio {

    class OutputSystem : public QObject {
        Q_OBJECT
    public:
        explicit OutputSystem(QObject *parent = nullptr);
        ~OutputSystem() override;

        bool initialize();

        talcs::AudioDriverManager *driverManager() const;
        talcs::AudioDriver *driver() const;
        talcs::AudioDevice *device() const;
        talcs::MixerAudioSource *preMixer() const;

        bool setDriver(const QString &driverName);
        bool setDevice(const QString &deviceName);
        bool enumerateDevices(bool saveToSettings);

        qint64 adoptedBufferSize() const;
        void setAdoptedBufferSize(qint64 bufferSize);
        double adoptedSampleRate() const;
        void setAdoptedSampleRate(double sampleRate);

        void testDevice();

        static QString driverDisplayName(const QString &driverName);

        enum HotPlugNotificationMode {
            Omni,
            Current,
            None,
        };
        HotPlugNotificationMode hotPlugNotificationMode() const;
        void setHotPlugNotificationMode(HotPlugNotificationMode mode);

    Q_SIGNALS:
        void adoptedBufferSizeChanged(qint64 bufferSize);
        void adoptedSampleRateChanged(double sampleRate);
        void deviceChanged();
        void deviceHotPlugged();

    private:
        talcs::AudioDriverManager *m_drvMgr;
        talcs::AudioDriver *m_drv = nullptr;
        std::unique_ptr<talcs::AudioDevice> m_dev;
        talcs::MixerAudioSource *m_preMixer;
        std::unique_ptr<talcs::AudioSourcePlayback> m_playback;

        qint64 m_adoptedBufferSize = 0;
        double m_adoptedSampleRate = 0.0;

        std::unique_ptr<QJsonObject> m_settingObj;

        HotPlugNotificationMode m_hotPlugNotificationMode = Omni;

        void handleDeviceHotPlug();
        void postSetDevice(bool saveToSettings);
    };

} // Audio

#endif // AUDIO_OUTPUTSYSTEM_H
