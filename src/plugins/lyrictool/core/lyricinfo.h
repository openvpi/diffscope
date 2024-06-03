#ifndef LYRICINFO_H
#define LYRICINFO_H

#include <QtCore/QSharedData>

#include <lyrictool/lyrictoolglobal.h>

namespace LyricTool {

    class LyricInfoData;

    class LYRICTOOL_EXPORT LyricInfo {
    public:
        LyricInfo();
        ~LyricInfo();

        LyricInfo(const LyricInfo &other);
        LyricInfo(LyricInfo &&other) noexcept;

        LyricInfo &operator=(const LyricInfo &other);
        LyricInfo &operator=(LyricInfo &&other) noexcept;

        inline bool isValid() const;

    public:
        QString lyric() const;
        void setLyric(const QString &lyric);

        QString syllable() const;
        void setSyllable(const QString &syllable);

        QString syllableRevised() const;
        void setSyllableRevised(const QString &syllableRevised);

        QStringList candidates() const;
        void setCandidates(const QStringList &candidates);

        QString language() const;
        void setLanguage(const QString &language);

        QString category() const;
        void setCategory(const QString &category);

        QString standard() const;
        void setStandard(const QString &standard);

        bool revised() const;
        void setRevised(bool revised);

        bool error() const;
        void setError(bool error);

    protected:
        QSharedDataPointer<LyricInfoData> d;
    };

    inline bool LyricInfo::isValid() const {
        return syllable().isEmpty();
    }

}

#endif // LYRICINFO_H
