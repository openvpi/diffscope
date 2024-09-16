#ifndef AUDIO_AUDIOEXPORTER_P_H
#define AUDIO_AUDIOEXPORTER_P_H

#include <audioplugin/audioexporter.h>

#include <QSharedData>

namespace talcs {
    class DspxProjectAudioExporter;
    class DspxProjectContext;
}

namespace Audio {

    class AudioExporterConfigData : public QSharedData {
    public:
        QString fileName;
        QString fileDirectory;
        AudioExporterConfig::FileType fileType;
        bool formatMono;
        int formatOption;
        int formatQuality;
        double formatSampleRate;
        AudioExporterConfig::MixingOption mixingOption;
        bool isMuteSoloEnabled;
        AudioExporterConfig::SourceOption sourceOption;
        QList<int> source;
        AudioExporterConfig::TimeRange timeRange;
    };

    class AudioExporterPrivate {
        Q_DECLARE_PUBLIC(AudioExporter)
    public:
        AudioExporter *q_ptr;
        AudioExporterConfig config;
        Core::IProjectWindow *windowHandle;

        QString projectName() const;
        QString projectDirectory() const;
        QString trackName(int trackIndex) const;
        talcs::DspxProjectContext *projectContext() const;
        QPair<int, int> calculateRange() const;
        QList<int> selectedSources() const;

        bool calculateTemplate(QString &templateString) const;
        bool calculateTemplate(QString &templateString, const QString &trackName, int trackIndex) const;

        AudioExporter::Warning warning;
        QStringList fileList;
        void updateFileListAndWarnings();

        talcs::DspxProjectAudioExporter *currentExporter = nullptr;
    };
}

#endif // AUDIO_AUDIOEXPORTER_P_H
