#ifndef AUDIO_OUTPUTSYSTEMINTERFACE_H
#define AUDIO_OUTPUTSYSTEMINTERFACE_H

#include <CoreApi/iexecutive.h>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class AudioSource;
    class AudioDriverManager;
    class AudioDriver;
    class AudioDevice;
    class MixerAudioSource;
}

namespace Audio {

    namespace Internal {
        class AudioPlugin;
    }

    class OutputSystemInterfacePrivate;

    class AUDIO_EXPORT OutputSystemInterface : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(OutputSystemInterface)
        friend class Internal::AudioPlugin;
    public:
        ~OutputSystemInterface() override;

        bool isVST() const;
        talcs::AudioDriverManager *driverManager() const;
        talcs::AudioDriver *driver() const;
        talcs::AudioDevice *device() const;
        talcs::MixerAudioSource *preMixer() const;

        bool isReady();

    Q_SIGNALS:
        void bufferSizeChanged(qint64 bufferSize);
        void sampleRateChanged(double sampleRate);
        void deviceChanged();

    private:
        explicit OutputSystemInterface(QObject *parent = nullptr);
        QScopedPointer<OutputSystemInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_OUTPUTSYSTEMINTERFACE_H
