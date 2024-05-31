#include "unknownanalyzer.h"

#include <QRandomGenerator>

namespace LyricTool::Internal {

    UnknownAnalyzer::UnknownAnalyzer(QObject *parent)
        : ILanguageAnalyzer(QStringLiteral("Unknown"), parent) {
        setDisplayName(tr("Unknown"));
        setDescription(tr("Capture Unknown characters."));
    }

    bool UnknownAnalyzer::contains(const QString &input) const {
        Q_UNUSED(input);
        return true;
    }

    QList<LyricInfo> UnknownAnalyzer::split(const QString &input) const {
        LyricInfo lyricInfo;
        lyricInfo.setLyric(input);
        return {lyricInfo};
    }

    QString UnknownAnalyzer::randString() const {
        const int unicode = QRandomGenerator::global()->bounded(0x2200, 0x22ff + 1);
        return {QChar(unicode)};
    }
}
