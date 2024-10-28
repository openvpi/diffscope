#include "lyricinfo.h"

#include <QStringList>

namespace LyricTool {

    class LyricInfoData : public QSharedData {
    public:
        QString lyric;
        QString syllable = QString();
        QString syllableRevised = QString();
        QStringList candidates = QStringList();
        QString language = QStringLiteral("Unknown");
        QString category = QStringLiteral("Unknown");
        QString standard = QStringLiteral("Unknown");
        bool revised = false;
        bool error = false;
    };

    LyricInfo::LyricInfo() : d(new LyricInfoData()) {
    }

    LyricInfo::~LyricInfo() = default;

    LyricInfo::LyricInfo(const LyricInfo &other) = default;

    LyricInfo::LyricInfo(LyricInfo &&other) noexcept : d(other.d) {
        other.d = nullptr;
    }

    LyricInfo &LyricInfo::operator=(const LyricInfo &other) = default;

    LyricInfo &LyricInfo::operator=(LyricInfo &&other) noexcept {
        qSwap(d, other.d);
        return *this;
    }

    QString LyricInfo::lyric() const {
        return d->lyric;
    }

    void LyricInfo::setLyric(const QString &lyric) {
        d->lyric = lyric;
    }

    QString LyricInfo::syllable() const {
        return d->syllable;
    }

    void LyricInfo::setSyllable(const QString &syllable) {
        d->syllable = syllable;
    }

    QString LyricInfo::syllableRevised() const {
        return d->syllableRevised;
    }

    void LyricInfo::setSyllableRevised(const QString &syllableRevised) {
        d->syllableRevised = syllableRevised;
    }

    QStringList LyricInfo::candidates() const {
        return d->candidates;
    }

    void LyricInfo::setCandidates(const QStringList &candidates) {
        d->candidates = candidates;
    }

    QString LyricInfo::language() const {
        return d->language;
    }

    void LyricInfo::setLanguage(const QString &language) {
        d->language = language;
    }

    QString LyricInfo::category() const {
        return d->language;
    }

    void LyricInfo::setCategory(const QString &category) {
        d->category = category;
    }

    QString LyricInfo::standard() const {
        return d->standard;
    }

    void LyricInfo::setStandard(const QString &standard) {
        d->standard = standard;
    }

    bool LyricInfo::revised() const {
        return d->revised;
    }

    void LyricInfo::setRevised(bool revised) {
        d->revised = revised;
    }

    bool LyricInfo::error() const {
        return d->error;
    }

    void LyricInfo::setError(bool error) {
        d->error = error;
    }

}