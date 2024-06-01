#include "kanaanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {
    KanaAnalyzer::KanaAnalyzer(QObject *parent)
        : ILanguageAnalyzer(QStringLiteral("Kana"), parent) {
        setDisplayName(tr("Kana"));
        setDescription(tr("Capture Kana characters."));
        setDiscardResult(true);
    }

    static QString specialKana = QStringLiteral("ャュョゃゅょァィゥェォぁぃぅぇぉ");

    static bool isKana(const QChar &c) {
        return ((c >= QChar(0x3040) && c <= QChar(0x309F)) ||
                (c >= QChar(0x30A0) && c <= QChar(0x30FF)));
    }

    static bool isSpecialKana(const QChar &c) {
        return specialKana.contains(c);
    }

    bool KanaAnalyzer::contains(const QString &input) const {
        for (const QChar &ch : input) {
            if (!(isKana(ch) && !isSpecialKana(ch))) {
                return false;
            }
        }
        return true;
    }

    QList<LyricInfo> KanaAnalyzer::split(const QString &input) const {
        QList<LyricInfo> results;

        int pos = 0;
        while (pos < input.length()) {
            const auto &currentChar = input[pos];
            LyricInfo note;
            if (isKana(currentChar)) {
                const int length =
                    pos + 1 < input.length() && isSpecialKana(input[pos + 1]) ? 2 : 1;
                note.lyric() = input.mid(pos, length);
                note.language() = id();
                note.category() = category();
                pos += length;
            } else {
                const int start = pos;
                while (pos < input.length() && !contains(input[pos])) {
                    pos++;
                }
                note.lyric() = input.mid(start, pos - start);
                note.language() = "Unknown";
                note.category() = "Unknown";
            }
            if (!note.lyric().isEmpty())
                results.append(note);
        }

        return results;
    }

    QString KanaAnalyzer::randString() const {
        const int unicode = QRandomGenerator::global()->bounded(0x3040, 0x309F + 1);
        const int unicode2 = QRandomGenerator::global()->bounded(0x30A0, 0x30FF + 1);

        QString result =
            QRandomGenerator::global()->bounded(2) == 0 ? QChar(unicode) : QChar(unicode2);
        if (QRandomGenerator::global()->bounded(2) == 0) {
            result += specialKana[QRandomGenerator::global()->bounded(
                static_cast<quint32>(specialKana.size()))];
        }
        return result;
    }
}