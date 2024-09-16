#ifndef AUDIO_AUDIOEXPORTER_H
#define AUDIO_AUDIOEXPORTER_H

#include <QObject>
#include <QSharedData>

#include <TalcsCore/ErrorStringProvider.h>

namespace Core {
    class IProjectWindow;
}

namespace Audio {
    class AudioExporter;

    class AudioExporterConfigData;
    class AudioExporterPrivate;

    class AudioExporterConfig {
    public:
        AudioExporterConfig();
        ~AudioExporterConfig();

        QString fileName() const;
        void setFileName(const QString &);

        QString fileDirectory() const;
        void setFileDirectory(const QString &);

        enum FileType {
            FT_Wav,
            FT_Flac,
            FT_OggVorbis,
            FT_Mp3,
        };
        FileType fileType() const;
        void setFileType(FileType);

        bool formatMono() const;
        void setFormatMono(bool);

        int formatOption() const;
        void setFormatOption(int);

        int formatQuality() const;
        void setFormatQuality(int);

        double formatSampleRate() const;
        void setFormatSampleRate(double);

        enum MixingOption {
            MO_Mixed,
            MO_Separated,
            MO_SeparatedThruMaster,
        };
        MixingOption mixingOption() const;
        void setMixingOption(MixingOption);

        bool isMuteSoloEnabled() const;
        void setMuteSoloEnabled(bool);

        enum SourceOption {
            SO_All,
            SO_Selected,
            SO_Custom,
        };
        SourceOption sourceOption() const;
        void setSourceOption(SourceOption);

        QList<int> source() const;
        void setSource(const QList<int> &);

        enum TimeRange {
            TR_All,
            TR_LoopSection,
        };
        TimeRange timeRange() const;
        void setTimeRange(TimeRange);

        QVariantMap toVariantMap() const;
        [[nodiscard]] static AudioExporterConfig fromVariantMap(const QVariantMap &map);

    private:
        QSharedDataPointer<AudioExporterConfigData> d;
    };

    class AudioExporterListener {
    public:
        virtual bool willStartCallback(AudioExporter *exporter) = 0;
        virtual void willFinishCallback(AudioExporter *exporter) = 0;
    };

    class AudioExporter : public QObject, public talcs::ErrorStringProvider {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioExporter)
    public:

        explicit AudioExporter(Core::IProjectWindow *window, QObject *parent = nullptr);
        ~AudioExporter() override;

        Core::IProjectWindow *windowHandle() const;

        [[nodiscard]] static QStringList presets();
        [[nodiscard]] static QList<QPair<QString, AudioExporterConfig>> predefinedPresets();
        [[nodiscard]] static AudioExporterConfig preset(const QString &name);
        static void addPreset(const QString &name, const AudioExporterConfig &config);
        static bool removePreset(const QString &name);

        static void registerListener(AudioExporterListener *listener);

        void setConfig(const AudioExporterConfig &config);
        AudioExporterConfig config() const;

        enum WarningFlag {
            W_NoFile = 0x0001,
            W_DuplicatedFile = 0x0002,
            W_WillOverwrite = 0x0004,
            W_UnrecognizedTemplate = 0x0008,
            W_LossyFormat = 0x00010,
        };
        Q_DECLARE_FLAGS(Warning, WarningFlag)
        Warning warning() const;
        [[nodiscard]]static QStringList warningText(Warning warning);

        QStringList dryRun() const;

        enum Result {
            R_OK,
            R_Fail,
            R_Abort,
        };
        Result exec();

        void cancel(bool isFail = false, const QString &message = {});

    signals:
        void progressChanged(double progressRatio, int sourceIndex);
        void clippingDetected(int sourceIndex);

    private:
        QScopedPointer<AudioExporterPrivate> d_ptr;
    };

} // Audio

#endif // AUDIO_AUDIOEXPORTER_H
