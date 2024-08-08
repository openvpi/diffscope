#ifndef AUDIO_AUDIOSETTINGS_H
#define AUDIO_AUDIOSETTINGS_H

#define AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(readName, writeName, valueType)                    \
  public:                                                                                          \
  [[nodiscard]] static valueType readName();                                                       \
  static void writeName(const valueType &);

#include <QJsonValue>

namespace Audio::Internal {

    class AudioSettings {
        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(adoptedBufferSize, setAdoptedBufferSize, qint64)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(adoptedSampleRate, setAdoptedSampleRate, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(deviceGain, setDeviceGain, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(deviceName, setDeviceName, QString)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(devicePan, setDevicePan, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(driverName, setDriverName, QString)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(fileBufferingReadAheadSize, setFileBufferingReadAheadSize, qint64)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(hotPlugNotificationMode, setHotPlugNotificationMode, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiDeviceIndex, setMidiDeviceIndex, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerAmplitude, setMidiSynthesizerAmplitude, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerAttackMsec, setMidiSynthesizerAttackMsec, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerDecayMsec, setMidiSynthesizerDecayMsec, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerDecayRatio, setMidiSynthesizerDecayRatio, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerFrequencyOfA, setMidiSynthesizerFrequencyOfA, double)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerGenerator, setMidiSynthesizerGenerator, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(midiSynthesizerReleaseMsec, setMidiSynthesizerReleaseMsec, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(playheadBehavior, setPlayheadBehavior, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(vstEditorPort, setVstEditorPort, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(vstPluginEditorUsesCustomTheme, setVstPluginEditorUsesCustomTheme, bool)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(vstPluginPort, setVstPluginPort, int)

        AUDIO_AUDIO_SETTINGS_OPTION_DECLARATION(vstTheme, setVstTheme, QJsonValue)

    };

}

#endif // AUDIO_AUDIOSETTINGS_H
