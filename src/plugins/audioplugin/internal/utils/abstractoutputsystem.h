#ifndef AUDIO_ABSTRACTOUTPUTSYSTEM_H
#define AUDIO_ABSTRACTOUTPUTSYSTEM_H

#include <memory>

#include <QObject>

#include "addonloader.h"
#include "ioutputsystemaddon.h"

namespace talcs {
    class MixerAudioSource;
    class AudioSourcePlayback;
    class AudioDevice;
}

namespace Audio {

    class AudioDeviceTesterAudioSource;

    class IOutputSystemAddOn;

    class AbstractOutputSystem : public QObject {
        Q_OBJECT
    public:
        explicit AbstractOutputSystem(QObject *parent = nullptr);
        ~AbstractOutputSystem() override;

        virtual bool initialize();
        virtual talcs::AudioDevice *device() const = 0;
        talcs::MixerAudioSource *preMixer() const;

        virtual bool makeReady() = 0;

        void setSubstitutedSource(talcs::AudioSource *source, IOutputSystemAddOn *substitutor);
        talcs::AudioSource *substitutedSource() const;
        IOutputSystemAddOn *sourceSubstitutor() const;
        void resetSubstitutedSource();
        talcs::AudioSource *currentSource() const;

        void setGainAndPan(float gain, float pan);
        float gain() const;
        float pan() const;

        float m_deviceGain = 1.0f;
        float m_devicePan = 0.0f;

    Q_SIGNALS:
        void bufferSizeChanged(qint64 bufferSize);
        void sampleRateChanged(double sampleRate);
        void deviceChanged();

    protected:
        talcs::MixerAudioSource *m_preMixer;
        std::unique_ptr<talcs::AudioSourcePlayback> m_playback;

        // TODO add-on modifier
        talcs::AudioSource *m_substitutedSource = nullptr;
        IOutputSystemAddOn *m_substitutor = nullptr;
    };

} // Audio

#endif // AUDIO_ABSTRACTOUTPUTSYSTEM_H
