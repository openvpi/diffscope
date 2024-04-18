#ifndef AUDIO_DEVICETESTERADDON_H
#define AUDIO_DEVICETESTERADDON_H

#include <audioplugin/ioutputsystemaddon.h>

namespace Audio {

    class AudioDeviceTesterAudioSource;

    class DeviceTesterAddOn : public IOutputSystemAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit DeviceTesterAddOn(QObject *parent = nullptr);
        ~DeviceTesterAddOn() override;

        static void testDevice(bool isVST);

        void initialize() override;
        void extensionInitialized() override;

    private:
        AudioDeviceTesterAudioSource *m_src;

    };

} // Audio

#endif // AUDIO_DEVICETESTERADDON_H
