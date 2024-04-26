#ifndef AUDIO_OUTPUTSYSTEMINTERFACE_H
#define AUDIO_OUTPUTSYSTEMINTERFACE_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class AudioSource;
    class AudioDriverManager;
    class AudioDriver;
    class AudioDevice;
    class MixerAudioSource;
}

namespace Audio {

    class AudioPlugin;

    class OutputSystemInterfacePrivate;

    class AUDIO_EXPORT OutputSystemInterface : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(OutputSystemInterface)
        friend class AudioPlugin;
    public:
        ~OutputSystemInterface() override;

        bool isVST() const;
        talcs::AudioDriverManager *audioDriverManager() const;
        talcs::AudioDriver *audioDriver() const;
        talcs::AudioDevice *audioDevice() const;
        talcs::MixerAudioSource *preMixer() const;

        bool makeReady();

    private:
        explicit OutputSystemInterface(QObject *parent = nullptr);
        QScopedPointer<OutputSystemInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_OUTPUTSYSTEMINTERFACE_H
