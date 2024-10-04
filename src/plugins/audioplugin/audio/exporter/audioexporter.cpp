#include "audioexporter.h"
#include "audioexporter_p.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QRegularExpression>
#include <QStandardPaths>

#include <TalcsCore/TransportAudioSource.h>
#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsFormat/AudioFormatIO.h>
#include <TalcsDspx/DspxProjectAudioExporter.h>
#include <TalcsDspx/DspxProjectContext.h>
#include <TalcsDspx/DspxTrackContext.h>

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/internal/audiosettings.h>
#include <audioplugin/internal/projectaddon.h>

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
    QStringList AudioExporterConfig::formatOptionsOfType(FileType type) {
        switch (type) {
            case FT_Wav:
                return {
                    QApplication::translate("Audio::AudioExporterConfig",
                                            "32-bit float (IEEE 754)"),
                    QApplication::translate("Audio::AudioExporterConfig", "24-bit PCM"),
                    QApplication::translate("Audio::AudioExporterConfig", "16-bit PCM"),
                    QApplication::translate("Audio::AudioExporterConfig", "Unsigned 8-bit PCM"),
                };
            case FT_Flac:
                return {
                    QApplication::translate("Audio::AudioExporterConfig", "24-bit PCM"),
                    QApplication::translate("Audio::AudioExporterConfig", "16-bit PCM"),
                    QApplication::translate("Audio::AudioExporterConfig", "8-bit PCM"),
                };
            default:
                return {};
        }
    }

    QString AudioExporterConfig::extensionOfType(FileType type) {
        switch (type) {
            case FT_Wav:
                return QStringLiteral("wav");
            case FT_Flac:
                return QStringLiteral("flac");
            case FT_OggVorbis:
                return QStringLiteral("ogg");
            case FT_Mp3:
                return QStringLiteral("mp3");
        }
        return {};
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
    bool AudioExporterConfig::operator==(const AudioExporterConfig &other) const {
        return d->fileName == other.d->fileName
            && d->fileDirectory == other.d->fileDirectory
            && d->fileType == other.d->fileType
            && d->formatMono == other.d->formatMono
            && d->formatOption == other.d->formatOption
            && d->formatQuality == other.d->formatQuality
            && d->formatSampleRate == other.d->formatSampleRate
            && d->mixingOption == other.d->mixingOption
            && d->isMuteSoloEnabled == other.d->isMuteSoloEnabled
            && d->sourceOption == other.d->sourceOption
            && d->timeRange == other.d->timeRange;
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
    talcs::DspxProjectContext *AudioExporterPrivate::projectContext() const {
        return ProjectAddOn::get(windowHandle)->projectContext();
    }
    QPair<int, int> AudioExporterPrivate::calculateRange() const {
        return {}; // TODO
    }
    QList<int> AudioExporterPrivate::selectedSources() const {
        return {}; // TODO
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
                replacedText = QString::number(config.formatSampleRate()).replace('.', '_');
            } else if (templateName == "today") {
                replacedText = QDate::currentDate().toString("yyyyMMdd");
            } else if (templateName == "$") {
                replacedText = QStringLiteral("$");
            } else if (trackIndex != -1) {
                if (templateName == "trackName") {
                    replacedText = trackName;
                } else if (templateName == "trackIndex") {
                    replacedText = QString::number(trackIndex + 1);
                } else {
                    allTemplatesMatch = false;
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

    int AudioExporterPrivate::calculateFormat() const {
        int format = 0;
        switch (config.fileType()) {
            case AudioExporterConfig::FT_Wav:
                format |= talcs::AudioFormatIO::WAV;
                switch (config.formatOption()) {
                    case 0:
                        format |= talcs::AudioFormatIO::FLOAT;
                        return format;
                    case 1:
                        format |= talcs::AudioFormatIO::PCM_24;
                        return format;
                    case 2:
                        format |= talcs::AudioFormatIO::PCM_16;
                        return format;
                    case 3:
                        format |= talcs::AudioFormatIO::PCM_U8;
                        return format;
                }
                break;
            case AudioExporterConfig::FT_Flac:
                format |= talcs::AudioFormatIO::FLAC;
                switch (config.formatOption()) {
                    case 0:
                        format |= talcs::AudioFormatIO::PCM_24;
                        return format;
                    case 1:
                        format |= talcs::AudioFormatIO::PCM_16;
                        return format;
                    case 2:
                        format |= talcs::AudioFormatIO::PCM_S8;
                        return format;
                }
                break;
            case AudioExporterConfig::FT_OggVorbis:
                format |= (talcs::AudioFormatIO::OGG | talcs::AudioFormatIO::VORBIS);
                return format;
            case AudioExporterConfig::FT_Mp3:
                format |= (talcs::AudioFormatIO::MPEG | talcs::AudioFormatIO::MPEG_LAYER_III);
                return format;
        }
        return 0;
    }

    void AudioExporterPrivate::updateFileListAndWarnings() {
        warning = {};
        if (config.fileType() == AudioExporterConfig::FT_Mp3 || config.fileType() == AudioExporterConfig::FT_OggVorbis)
            warning |= AudioExporter::W_LossyFormat;
        fileList.clear();
        if (config.mixingOption() == AudioExporterConfig::MO_Mixed) {
            auto calculatedFileName = config.fileName();
            if (!calculateTemplate(calculatedFileName))
                warning |= AudioExporter::W_UnrecognizedTemplate;
            auto fileInfo = QFileInfo(QDir(QDir(projectDirectory()).absoluteFilePath(config.fileDirectory())).absoluteFilePath(calculatedFileName));
            if (fileInfo.exists())
                warning |= AudioExporter::W_WillOverwrite;
            fileList.append(fileInfo.absoluteFilePath());
        } else {
            if (config.source().isEmpty())
                warning |= AudioExporter::W_NoFile;
            QSet<QString> fileSet;
            for (auto index : config.source()) {
                auto calculatedFileName = config.fileName();
                if (!calculateTemplate(calculatedFileName, trackName(index), index))
                    warning |= AudioExporter::W_UnrecognizedTemplate;
                auto fileInfo = QFileInfo(QDir(QDir(projectDirectory()).absoluteFilePath(config.fileDirectory())).absoluteFilePath(calculatedFileName));
                if (fileInfo.exists())
                    warning |= AudioExporter::W_WillOverwrite;
                if (fileSet.contains(fileInfo.absoluteFilePath()))
                    warning |= AudioExporter::W_DuplicatedFile;
                fileSet.insert(fileInfo.absoluteFilePath());
                fileList.append(fileInfo.absoluteFilePath());
            }
        }
    }

    AudioExporter::AudioExporter(Core::IProjectWindow *window, QObject *parent) : QObject(parent), d_ptr(new AudioExporterPrivate) {
        Q_D(AudioExporter);
        d->q_ptr = this;
        d->windowHandle = window;
    }
    AudioExporter::~AudioExporter() = default;

    Core::IProjectWindow *AudioExporter::windowHandle() const {
        Q_D(const AudioExporter);
        return d->windowHandle;
    }

    QStringList AudioExporter::presets() {
        return AudioSettings::audioExporterPresets().toObject().keys();
    }

    QList<QPair<QString, AudioExporterConfig>> AudioExporter::predefinedPresets() {
        static auto wavMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.wav"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Wav},
            {"formatMono", false},
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
            {"formatMono", false},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_SeparatedThruMaster},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto flacMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.flac"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_Flac},
            {"formatMono", false},
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
            {"formatMono", false},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_SeparatedThruMaster},
            {"isMuteSoloEnabled", true},
            {"sourceOption", AudioExporterConfig::SO_All},
            {"source", {}},
            {"timeRange", AudioExporterConfig::TR_All},
        });
        static auto oggMix = AudioExporterConfig::fromVariantMap({
            {"fileName", "${projectName}.ogg"},
            {"fileDirectory", {}},
            {"fileType", AudioExporterConfig::FT_OggVorbis},
            {"formatMono", false},
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
            {"formatMono", false},
            {"formatOption", 0},
            {"formatQuality", 100},
            {"formatSampleRate", 48000},
            {"mixingOption", AudioExporterConfig::MO_SeparatedThruMaster},
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
            list.append(tr("The files to be exported contain files with the same name as existing files. If continue, the existing files will be overwritten."));
        }
        if (warning & W_UnrecognizedTemplate) {
            list.append(tr("Unrecognized file name template. Please check the syntax of the file name template."));
        }
        if (warning & W_LossyFormat) {
            list.append(tr("The currently selected file type is a lossy format. To avoid loss of sound quality, please use WAV or FLAC format."));
        }
        return list;
    }

    QStringList AudioExporter::dryRun() const {
        Q_D(const AudioExporter);
        return d->fileList;
    }

    AudioExporter::Result AudioExporter::exec() {
        Q_D(AudioExporter);

        const auto config = this->config();
        auto projectContext = d->projectContext();

        clearErrorString();

        // prepare AudioFormatIO for exporting
        QObject o;
        std::unique_ptr<talcs::AudioFormatIO[]> ioList(new talcs::AudioFormatIO[d->fileList.size()]);
        for (int i = 0; i < d->fileList.size(); i++) {
            auto file = new QFile(d->fileList[i], &o);
            if (!file->open(QIODevice::WriteOnly)) {
                setErrorString(tr("Cannot open file for writing: %1").arg(d->fileList[i]));
                return R_Fail;
            }
            auto &io = ioList[i];
            io.setStream(file);
            io.setSampleRate(config.formatSampleRate());
            io.setChannelCount(config.formatMono() ? 1 : 2);
            io.setFormat(d->calculateFormat());
            if (!io.open(talcs::AbstractAudioFormatIO::Write)) {
                setErrorString(tr("Format not supported: %1").arg(io.errorString()));
                return R_Fail;
            }
            io.setCompressionLevel(0.01 * (100 - config.formatQuality()));
        }

        // create and configure talcs::DspxProjectAudioExporter
        talcs::DspxProjectAudioExporter exporter(projectContext);
        auto cleanup = [=](void *) {d->currentExporter = nullptr;};
        std::unique_ptr<void, decltype(cleanup)> _1(this, cleanup);
        d->currentExporter = &exporter;
        exporter.setMonoChannel(config.formatMono());
        exporter.setThruMaster(config.mixingOption() == AudioExporterConfig::MO_SeparatedThruMaster);
        exporter.setClippingCheckEnabled(AudioSettings::audioExporterClippingCheckEnabled());
        exporter.setMuteSoloEnabled(config.isMuteSoloEnabled());
        auto range = d->calculateRange();
        exporter.setRange(range.first, range.second);
        QList<talcs::DspxTrackContext *> tracks;
        switch (config.sourceOption()) {
            case AudioExporterConfig::SO_All:
                tracks = projectContext->tracks();
                break;
            case AudioExporterConfig::SO_Selected:
            case AudioExporterConfig::SO_Custom:
                for (auto index : config.source()) {
                    tracks.append(projectContext->tracks().at(index));
                }
                break;
        }
        if (config.mixingOption() == AudioExporterConfig::MO_Mixed) {
            exporter.setMixedTask(tracks, &ioList[0]);
        } else {
            for (int i = 0; i < tracks.size(); i++) {
                exporter.addSeparatedTask(tracks[i], &ioList[i]);
            }
        }
        QHash<talcs::DspxTrackContext *, int> sourceIndexMap;
        sourceIndexMap.insert(nullptr, -1);
        for (int i = 0; i < tracks.size(); i++) {
            sourceIndexMap.insert(tracks[i], i);
        }

        // deal with audio components
        projectContext->transport()->pause();
        auto currentBufferSize = projectContext->preMixer()->bufferSize();
        auto currentSampleRate = projectContext->preMixer()->sampleRate();
        auto reopenMixer = [=](void *) {
            if (!projectContext->preMixer()->open(currentBufferSize, currentSampleRate))
                qDebug() << "AudioExporter: Cannot reopen pre-mixer after exported";
        };
        std::unique_ptr<void, decltype(reopenMixer)> _2(this, reopenMixer);
        if (!projectContext->preMixer()->open(1024, config.formatSampleRate())) { // TODO let user configure buffer size in settings
            setErrorString(tr("Cannot start audio exporting"));
            return R_Fail;
        }

        // call listeners
        QList<AudioExporterListener *> listenerToCallFinishList;
        auto callFinish = [&](void *) {
            for (auto listener : listenerToCallFinishList) {
                listener->willFinishCallback(this);
            }
        };
        std::unique_ptr<void, decltype(callFinish)> _3(this, callFinish);
        // Note: order of destruction: call AudioExporterListener::willFinish after mixer reopened
        std::unique_ptr<void, decltype(reopenMixer)> _4 = std::move(_2);
        for (auto listener : m_listeners) {
            if (!listener->willStartCallback(this))
                return R_Fail;
            listenerToCallFinishList.prepend(listener);
        }

        // start exporting
        connect(&exporter, &talcs::DspxProjectAudioExporter::progressChanged, this, [=](double progressRatio, talcs::DspxTrackContext *track) {
            emit progressChanged(progressRatio, sourceIndexMap.value(track));
        });
        connect(&exporter, &talcs::DspxProjectAudioExporter::clippingDetected, this, [=](talcs::DspxTrackContext *track) {
            emit clippingDetected(sourceIndexMap.value(track));
        });
        auto ret = exporter.exec();
        if (ret == talcs::DspxProjectAudioExporter::OK)
            return R_OK;
        if (ret == talcs::DspxProjectAudioExporter::Interrupted)
            return R_Abort;

        if (errorString().isEmpty())
            setErrorString(tr("Internal Error"));
        return R_Fail;
    }

    void AudioExporter::cancel(bool isFail, const QString &message) {
        Q_D(AudioExporter);
        if (!d->currentExporter)
            return;
        if (isFail)
            setErrorString(message.isEmpty() ? tr("Unknown error") : message);
        d->currentExporter->interrupt(isFail);
    }

    void AudioExporter::addWarning(const QString &message, int sourceIndex) {
        emit warningAdded(message, sourceIndex);
    }
} // Audio
