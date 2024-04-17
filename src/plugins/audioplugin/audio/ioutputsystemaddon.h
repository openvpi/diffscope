#ifndef AUDIO_IOUTPUTSYSTEMADDON_H
#define AUDIO_IOUTPUTSYSTEMADDON_H

#include <QObject>

namespace talcs {
    class AudioDriverManager;
    class AudioDriver;
    class AudioDevice;
    class MixerAudioSource;
}

namespace Audio {

    class IOutputSystemAddOn : public QObject {
        Q_OBJECT
    public:
        explicit IOutputSystemAddOn(QObject *parent = nullptr);
        ~IOutputSystemAddOn() override;

        virtual bool initialize() = 0;
        virtual void extensionInitialized() = 0;
        virtual bool delayedInitialize();

        virtual void handleBufferSizeChange();
        virtual void handleSampleRateChange();
        virtual void handleDeviceChange();

        bool isVST() const;
        talcs::AudioDriverManager *audioDriverManager() const;
        talcs::AudioDriver *audioDriver() const;
        talcs::AudioDevice *audioDevice() const;
        talcs::MixerAudioSource *preMixer() const;

        bool makeReady();

    };

} // Audio

#endif // AUDIO_IOUTPUTSYSTEMADDON_H
