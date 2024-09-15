#include "audioexporter.h"
#include "audioexporter_p.h"
#include "../../../coreplugin/windows/iprojectwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QRegularExpression>
#include <QStandardPaths>

#include <CoreApi/iloader.h>

#include <audioplugin/internal/audiosettings.h>

namespace Audio {
    using namespace Internal;

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
    bool AudioExporterConfig::formatMono() const {
        return d->formatMono;
    }
    void AudioExporterConfig::setFormatMono(bool a_) {
        d->formatMono = a_;
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
            {"formatMono", d->formatMono},
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
        config.d->formatMono = map.value("formatMono").toBool();
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

    QString AudioExporterPrivate::projectName() const {
        // project file's base name
        return QFileInfo(windowHandle->doc()->filePath()).baseName();
    }
    QString AudioExporterPrivate::projectDirectory() const {
        if (auto dir = QFileInfo(windowHandle->doc()->filePath()).dir(); dir.isRelative()) {
            return QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
        } else {
            return dir.path();
        }
    }
    QString AudioExporterPrivate::trackName(int trackIndex) const {
        return windowHandle->doc()->dataModel().model()->tracks()->at(trackIndex)->trackName();
    }
    bool AudioExporterPrivate::calculateTemplate(QString &templateString) const {
        return calculateTemplate(templateString, {}, -1);
    }
    bool AudioExporterPrivate::calculateTemplate(QString &templateString, const QString &trackName,
                                                 int trackIndex) const {
        static QRegularExpression re(R"re(\$\{(.*?)\})re");
        bool allTemplatesMatch = true;
        auto templateStringView = QStringView(templateString);
        int pos = 0;
        QString result;
        for (auto matchIt = re.globalMatch(templateStringView); matchIt.hasNext(); matchIt.next()) {
            auto match = matchIt.peekNext();
            auto templateName = match.captured(1);
            auto replacedText = match.captured(0);
            if (templateName == "projectName") {
                replacedText = projectName();
            } else if (templateName == "sampleRate") {
                replacedText = QString::number(config.formatSampleRate());
            } else if (templateName == "today") {
                replacedText = QDate::currentDate().toString("yyyyMMdd");
            } else if (templateName == "$") {
                replacedText = QStringLiteral("$");
            } else if (trackIndex != -1) {
                if (templateName == "trackName") {
                    replacedText = trackName;
                } else if (templateName == "trackIndex") {
                    replacedText = QString::number(trackIndex + 1);
                }
            } else {
                allTemplatesMatch = false;
            }
            result += templateStringView.mid(pos, match.capturedStart(0) - pos);
            result += replacedText;
            pos = match.capturedEnd(0);
        }
        result += templateStringView.right(templateStringView.length() - pos);
        templateString = result;
        return allTemplatesMatch;
    }
    void AudioExporterPrivate::updateFileListAndWarnings() {
        warning = {};
        if (config.fileType() == AudioExporterConfig::FT_Mp3 || config.fileType() == AudioExporterConfig::FT_OggVorbis)
            warning |= AudioExporter::W_LossyFormat;
        fileList.clear();
        if (config.sourceOption() == AudioExporterConfig::SO_All) {
            auto calculatedFileName = config.fileName();
            if (!calculateTemplate(calculatedFileName))
                warning |= AudioExporter::W_UnrecognizedTemplate;
            auto fileInfo = QFileInfo(QDir(projectDirectory()).absoluteFilePath(calculatedFileName));
            if (fileInfo.exists())
                warning |= AudioExporter::W_WillOverwrite;
            fileList.append(fileInfo.canonicalFilePath());
        } else {
            if (config.source().isEmpty())
                warning |= AudioExporter::W_NoFile;
            QSet<QString> fileSet;
            for (auto index : config.source()) {
                auto calculatedFileName = config.fileName();
                if (!calculateTemplate(calculatedFileName, trackName(index), index))
                    warning |= AudioExporter::W_UnrecognizedTemplate;
                auto fileInfo = QFileInfo(QDir(projectDirectory()).absoluteFilePath(calculatedFileName));
                if (fileInfo.exists())
                    warning |= AudioExporter::W_WillOverwrite;
                if (fileSet.contains(fileInfo.canonicalFilePath()))
                    warning |= AudioExporter::W_DuplicatedFile;
                fileSet.insert(fileInfo.canonicalFilePath());
            }
            fileList.append(fileSet.values());
        }
    }

    AudioExporter::AudioExporter(Core::IProjectWindow *window, QObject *parent) : QObject(parent), d_ptr(new AudioExporterPrivate) {
        Q_D(AudioExporter);
        d->q_ptr = this;
        d->windowHandle = window;
    }
    AudioExporter::~AudioExporter() = default;

    QStringList AudioExporter::presets() {
        return AudioSettings::audioExporterPresets().toObject().keys();
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
        return AudioExporterConfig::fromVariantMap(AudioSettings::audioExporterPresets()[name].toObject().toVariantMap());
    }

    void AudioExporter::addPreset(const QString &name, const AudioExporterConfig &config) {
        auto presetsObj = AudioSettings::audioExporterPresets().toObject();
        presetsObj.insert(name, QJsonObject::fromVariantMap(config.toVariantMap()));
        AudioSettings::setAudioExporterPresets(presetsObj);
    }

    bool AudioExporter::removePreset(const QString &name) {
        auto presetsObj = AudioSettings::audioExporterPresets().toObject();
        if (!presetsObj.contains(name))
            return false;
        presetsObj.remove(name);
        AudioSettings::setAudioExporterPresets(presetsObj);
        return true;
    }

    static QList<AudioExporterListener *> m_listeners;

    void AudioExporter::registerListener(AudioExporterListener *listener) {
        m_listeners.append(listener);
    }

    void AudioExporter::setConfig(const AudioExporterConfig &config) {
        Q_D(AudioExporter);
        d->config = config;
        d->updateFileListAndWarnings();
    }
    AudioExporterConfig AudioExporter::config() const {
        Q_D(const AudioExporter);
        return d->config;
    }

    AudioExporter::Warning AudioExporter::warning() const {
        Q_D(const AudioExporter);
        return d->warning;
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
        Q_D(const AudioExporter);
        return d->fileList;
    }

    AudioExporter::Result AudioExporter::exec() {
        return {};
    }

    void AudioExporter::cancel(bool isFail) {

    }
} // Audio
