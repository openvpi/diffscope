#ifndef AUDIO_OUTPUTSYSTEM_H
#define AUDIO_OUTPUTSYSTEM_H

#include <memory>

#include "abstractoutputsystem.h"

namespace talcs {
    class AudioDriver;
    class AudioDriverManager;
}

namespace Audio::Internal {

    class OutputSystem : public AbstractOutputSystem {
        Q_OBJECT
    public:
        explicit OutputSystem(QObject *parent = nullptr);
        ~OutputSystem() override;

        bool initialize() override;

        talcs::AudioDriverManager *driverManager() const;
        talcs::AudioDriver *driver() const;
        talcs::AudioDevice *device() const override;
        talcs::MixerAudioSource *preMixer() const;

        bool setDriver(const QString &driverName);
        bool setDevice(const QString &deviceName);
        bool enumerateDevices(bool saveToSettings);

        qint64 adoptedBufferSize() const;
        void setAdoptedBufferSize(qint64 bufferSize);
        double adoptedSampleRate() const;
        void setAdoptedSampleRate(double sampleRate);

        bool makeReady() override;

        static QString driverDisplayName(const QString &driverName);

        enum HotPlugNotificationMode {
            Omni,
            Current,
            None,
        };
        Q_ENUM(HotPlugNotificationMode)
        HotPlugNotificationMode hotPlugNotificationMode() const;
        void setHotPlugNotificationMode(HotPlugNotificationMode mode);

    Q_SIGNALS:
        void deviceHotPlugged();

    private:
        talcs::AudioDriverManager *m_drvMgr;
        talcs::AudioDriver *m_drv = nullptr;
        std::unique_ptr<talcs::AudioDevice> m_dev;

        qint64 m_adoptedBufferSize = 0;
        double m_adoptedSampleRate = 0.0;

        HotPlugNotificationMode m_hotPlugNotificationMode = Omni;

        void handleDeviceHotPlug();
        void postSetDevice(bool saveToSettings);
    };

}

#endif // AUDIO_OUTPUTSYSTEM_H
