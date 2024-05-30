#include "audioexporter.h"
#include "audioexporter_p.h"

#include <QVariant>

#include <CoreApi/iloader.h>

namespace Audio {

    AudioExporterConfig::AudioExporterConfig() : d(new AudioExporterConfigData) {
    }
    AudioExporterConfig::~AudioExporterConfig() = default;

    QString AudioExporterConfig::fileName() const {
        return d->fileName;
    }
    void AudioExporterConfig::setFileName(const QString &a_) {
        d->fileName = a_;
    }
    QString AudioExporterConfig::fileDirectory() const {
        return d->fileDirectory;
    }
    void AudioExporterConfig::setFileDirectory(const QString &a_) {
        d->fileDirectory = a_;
    }
    AudioExporterConfig::FileType AudioExporterConfig::fileType() const {
        return d->fileType;
    }
    void AudioExporterConfig::setFileType(AudioExporterConfig::FileType a_) {
        d->fileType = a_;
    }
    int AudioExporterConfig::formatOption() const {
        return d->formatOption;
    }
    void AudioExporterConfig::setFormatOption(int a_) {
        d->formatOption = a_;
    }
    int AudioExporterConfig::formatQuality() const {
        return d->formatQuality;
    }
    void AudioExporterConfig::setFormatQuality(int a_) {
        d->formatQuality = a_;
    }
    double AudioExporterConfig::formatSampleRate() const {
        return d->formatSampleRate;
    }
    void AudioExporterConfig::setFormatSampleRate(double a_) {
        d->formatSampleRate = a_;
    }
    AudioExporterConfig::MixingOption AudioExporterConfig::mixingOption() const {
        return d->mixingOption;
    }
    void AudioExporterConfig::setMixingOption(AudioExporterConfig::MixingOption a_) {
        d->mixingOption = a_;
    }
    bool AudioExporterConfig::isMuteSoloEnabled() const {
        return d->isMuteSoloEnabled;
    }
    void AudioExporterConfig::setMuteSoloEnabled(bool a_) {
        d->isMuteSoloEnabled = a_;
    }
    AudioExporterConfig::SourceOption AudioExporterConfig::sourceOption() const {
        return d->sourceOption;
    }
    void AudioExporterConfig::setSourceOption(AudioExporterConfig::SourceOption a_) {
        d->sourceOption = a_;
    }
    QList<int> AudioExporterConfig::source() const {
        return d->source;
    }
    void AudioExporterConfig::setSource(const QList<int> &a_) {
        d->source = a_;
    }
    AudioExporterConfig::TimeRange AudioExporterConfig::timeRange() const {
        return d->timeRange;
    }
    void AudioExporterConfig::setTimeRange(AudioExporterConfig::TimeRange a_) {
        d->timeRange = a_;
    }

    QVariantMap AudioExporterConfig::toVariantMap() const {
        return QVariantMap({
            {"fileName", d->fileName},
            {"fileDirectory", d->fileDirectory},
            {"fileType", d->fileType},
            {"formatOption", d->formatOption},
            {"formatQuality", d->formatQuality},
            {"formatSampleRate", d->formatSampleRate},
            {"mixingOption", d->mixingOption},
            {"isMuteSoloEnabled", d->isMuteSoloEnabled},
            {"sourceOption", d->sourceOption},
            {"source", QVariant::fromValue(d->source)},
            {"timeRange", d->timeRange},
        });
    }
    AudioExporterConfig AudioExporterConfig::fromVariantMap(const QVariantMap &map) {
        AudioExporterConfig config;
        config.d->fileName = map.value("fileName").toString();
        config.d->fileDirectory = map.value("fileDirectory").toString();
        config.d->fileType = static_cast<FileType>(map.value("fileType").toInt());
        config.d->formatOption = map.value("formatOption").toInt();
        config.d->formatQuality = map.value("formatQuality").toInt();
        config.d->formatSampleRate = map.value("formatSampleRate").toDouble();
        config.d->mixingOption = static_cast<MixingOption>(map.value("mixingOption").toInt());
        config.d->isMuteSoloEnabled = map.value("isMuteSoloEnabled").toBool();
        config.d->sourceOption = static_cast<SourceOption>(map.value("sourceOption").toInt());
        config.d->source = map.value("source").value<QList<int>>();
        config.d->timeRange = static_cast<TimeRange>(map.value("timeRange").toInt());
        return config;
    }

    AudioExporter::AudioExporter(Core::IProjectWindow *window, QObject *parent) : QObject(parent), d_ptr(new AudioExporterPrivate) {
        Q_D(AudioExporter);
        d->q_ptr = this;
        d->windowHandle = window;
    }
    AudioExporter::~AudioExporter() = default;

    QStringList AudioExporter::presets() {
        auto &settings = *Core::ILoader::instance()->settings();
        const auto obj = settings["Audio"].toObject();
        return obj["audioExporterPresets"].toObject().keys();
    }

    QList<QPair<QString, AudioExporterConfig>> AudioExporter::predefinedPresets() {
        static auto wavMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.wav"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Wav},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Mixed},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto wavSep = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}_${trackIndex}_${trackName}.wav"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Wav},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Separated},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto flacMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.flac"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Flac},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Mixed},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto flacSep = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}_${trackIndex}_${trackName}.flac"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Flac},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Separated},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto oggMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.ogg"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_OggVorbis},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Mixed},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto oggSep = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}_${trackIndex}_${trackName}.ogg"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_OggVorbis},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_Separated},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        return {
            {tr("WAV - Mixed"), wavMix},
            {tr("WAV - Separated"), wavSep},
            {tr("FLAC - Mixed"), flacMix},
            {tr("FLAC - Separated"), flacSep},
            {tr("Ogg/Vorbis - Mixed"), oggMix},
            {tr("Ogg/Vorbis - Separated"), oggSep},
        };
    }

    AudioExporterConfig AudioExporter::preset(const QString &name) {
        auto &settings = *Core::ILoader::instance()->settings();
        const auto obj = settings["Audio"].toObject();
        return AudioExporterConfig::fromVariantMap(obj["audioExporterPresets"][name].toObject().toVariantMap());
    }

    void AudioExporter::addPreset(const QString &name, const AudioExporterConfig &config) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        auto presetsObj = obj["audioExporterPresets"].toObject();
        presetsObj.insert(name, QJsonObject::fromVariantMap(config.toVariantMap()));
        obj["audioExporterPresets"] = presetsObj;
        settings["Audio"] = obj;
    }

    bool AudioExporter::removePreset(const QString &name) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        auto presetsObj = obj["audioExporterPresets"].toObject();
        if (!presetsObj.contains(name))
            return false;
        presetsObj.remove(name);
        obj["audioExporterPresets"] = presetsObj;
        settings["Audio"] = obj;
        return true;
    }

    void AudioExporter::setConfig(const AudioExporterConfig &config) {
        Q_D(AudioExporter);
        d->config = config;
    }
    AudioExporterConfig AudioExporter::config() const {
        Q_D(const AudioExporter);
        return d->config;
    }

    AudioExporter::Warning AudioExporter::warning() const {
        return Audio::AudioExporter::Warning();
    }

    QStringList AudioExporter::warningText(AudioExporter::Warning warning) {
        QStringList list;
        if (warning & W_NoFile) {
            list.append(tr("No file will be exported. Please check if any source is selected."));
        }
        if (warning & W_DuplicatedFile) {
            list.append(tr("The files to be exported contain files with duplicate names. Please check if the file name template is unique for each source."));
        }
        if (warning & W_WillOverwrite) {
            list.append(tr("The files to be exported contain files with the same name as existing files."));
        }
        if (warning & W_UnrecognizedTemplate) {
            list.append(tr("Unrecognized file name template. Please check the syntax of the file name template."));
        }
        if (warning & W_LossyFormat) {
            list.append(tr("Currently selected file type is a lossy format. Please use WAV or FLAC format to avoid loss of sound quality."));
        }
        return list;
    }

    QStringList AudioExporter::dryRun() const {
        return QStringList();
    }

    int AudioExporter::exec() {
        return 0;
    }
} // Audio
