#include "romajianalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {

    RomajiAnalyzer::RomajiAnalyzer(QObject *parent)
        : ILanguageAnalyzer(QStringLiteral("Romaji"), parent) {
        setDisplayName(tr("Mandarin"));
        setDescription(tr("Capture Mandarin characters."));
        loadDict();
    }

    void RomajiAnalyzer::loadDict() {
        QStringList initial = {"k",  "g",  "s",  "z",  "t",  "d",  "n",  "h",  "b",
                               "p",  "m",  "y",  "r",  "w",  "ky", "gy", "sh", "j",
                               "ch", "ny", "hy", "by", "py", "my", "ry"};
        QStringList final = {"a", "i", "u", "e", "o"};
        QStringList special = {"n", "nn", "m"};

        for (const auto &i : initial) {
            for (const auto &f : final) {
                romajiSet.insert(i + f);
            }
        }

        for (const auto &s : special) {
            romajiSet.insert(s);
        }

        for (const auto &i : initial) {
            romajiSet.insert(i);
        }

        for (const auto &f : final) {
            romajiSet.insert(f);
        }
    }

    static bool isLetter(QChar c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\'';
    }

    QList<LyricInfo> RomajiAnalyzer::split(const QString &input) const {
        QList<LyricInfo> result;

        int pos = 0;
        while (pos < input.length()) {
            const auto &currentChar = input[pos];
            LyricInfo note;
            if (isLetter(currentChar)) {
                const int start = pos;
                while (pos < input.length() && isLetter(input[pos])) {
                    pos++;
                }

                note.lyric() = input.mid(start, pos - start);
                if (contains(note.lyric())) {
                    note.language() = id();
                    note.category() = category();
                } else {
                    note.language() = QStringLiteral("English");
                    note.category() = QStringLiteral("English");
                }
            } else {
                const int start = pos;
                while (pos < input.length() && !isLetter(input[pos])) {
                    pos++;
                }
                note.lyric() = input.mid(start, pos - start);
                note.language() = QStringLiteral("Unknown");
                note.category() = QStringLiteral("Unknown");
            }
            if (!note.lyric().isEmpty())
                result.append(note);
        }
        return result;
    }

}