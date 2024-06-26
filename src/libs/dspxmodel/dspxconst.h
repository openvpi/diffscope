#ifndef DSPXCONST_H
#define DSPXCONST_H

namespace QDspx {

    // Metadata
    const char Version[] = "1.0.0";
    const char DefaultAuthor[] = "";
    const char DefaultProjectName[] = "New Project";
    const int DefaultCentShift = 0;

    // Control
    const double DefaultGain = 0;
    const double DefaultPan = 0;
    const bool DefaultMute = false;
    const bool DefaultSolo = false;

    // Timeline
    const int DefaultTimeSignatureBarIndex = 0;
    const int DefaultTimeSignatureNumerator = 4;
    const int DefaultTimeSignatureDenominator = 4;

    // Tempo
    const int DefaultTempoPos = 0;
    const double DefaultTempoValue = 120.0;

    // Label
    const int DefaultLabelPos = 0;
    const char DefaultLabelText[] = "";

    // Track
    const char DefaultTrackName[] = "Untitled Track";

    // Clip
    const char DefaultSingingClipName[] = "Untitled Clip"; // To remove
    const char DefaultAudioClipName[] = "Untitled Clip";   // To remove
    const char DefaultAudioClipPath[] = "";
    const char DefaultClipName[] = "Untitled Clip";
    const int DefaultClipStart = 0;
    const int DefaultClipLength = 480;
    const int DefaultClipClipStart = 0;
    const int DefaultClipClipLength = 480;

    // Note
    const int DefaultNotePos = 0;
    const int DefaultNoteLength = 480;
    const int DefaultNoteKeyNum = 60;
    const char DefaultNoteLyric[] = "la";
    const char DefaultNotePronunciation[] = "";
    const char DefaultNoteOrgPronunciation[] = "la";

    // Vibrato
    const double DefaultVibratoStart = 0;
    const double DefaultVibratoEnd = 1;
    const double DefaultVibratoFrequency = 5;
    const double DefaultVibratoPhase = 0;
    const double DefaultVibratoAmplitude = 1;
    const double DefaultVibratoOffset = 0;

    // Phoneme
    const char DefaultPhonemeType[] = "ahead";
    const char DefaultPhonemeToken[] = "a";
    const int DefaultPhonemeStart = 0;

    // ParamAnchor
    const char DefaultParamAnchorNodeInterpolation[] = "linear";

    // ParamFree
    const int DefaultParamCurveStart = 0;
    const int DefaultParamFreeStart = 0; // To remove
    const int DefaultParamFreeStep = 5;

}

#endif // DSPXCONST_H
