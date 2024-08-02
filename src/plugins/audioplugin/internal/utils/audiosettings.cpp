#include "audiosettings.h"

#include <QJsonObject>
#include <QVariant>

#include <CoreApi/iloader.h>

namespace Audio::Internal {
    
    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(adoptedBufferSize, setAdoptedBufferSize, qint64)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(adoptedSampleRate, setAdoptedSampleRate, double)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(deviceGain, setDeviceGain, double, 1.0)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(deviceName, setDeviceName, QString)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(devicePan, setDevicePan, double)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(driverName, setDriverName, QString)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(fileBufferingReadAheadSize, setFileBufferingReadAheadSize, qint64)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(hotPlugNotificationMode, setHotPlugNotificationMode, int)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(midiDeviceIndex, setMidiDeviceIndex, int, -1)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(midiSynthesizerAmplitude, setMidiSynthesizerAmplitude, double, -3)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(midiSynthesizerAttackMsec, setMidiSynthesizerAttackMsec, int, 10)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(midiSynthesizerDecayMsec, setMidiSynthesizerDecayMsec, int)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(midiSynthesizerDecayRatio, setMidiSynthesizerDecayRatio, double, 1.0)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(midiSynthesizerFrequencyOfA, setMidiSynthesizerFrequencyOfA, double)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(midiSynthesizerGenerator, setMidiSynthesizerGenerator, int)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(midiSynthesizerReleaseMsec, setMidiSynthesizerReleaseMsec, int, 50)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(vstEditorPort, setVstEditorPort, int, 28081)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_1(vstPluginEditorUsesCustomTheme, setVstPluginEditorUsesCustomTheme, bool)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_2(vstPluginPort, setVstPluginPort, int, 28082)

    AUDIO_AUDIO_SETTINGS_OPTION_IMPLEMENTATION_QJSONVALUE(vstTheme, setVstTheme)
    
} // Audio