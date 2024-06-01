#include "multicharanalyzer.h"
#include "multicharanalyzer_p.h"

namespace LyricTool {
    MultiCharAnalyzerPrivate::MultiCharAnalyzerPrivate() {
    }

    void MultiCharAnalyzerPrivate::init() {
    }

    MultiCharAnalyzer::MultiCharAnalyzer(const QString &id, QObject *parent)
        : ILanguageAnalyzer(id, parent) {
    }

    QList<LyricInfo> MultiCharAnalyzer::split(const QString &input) const {
        QList<LyricInfo> result;

        int pos = 0;
        while (pos < input.length()) {
            const auto &currentChar = input[pos];
            LyricInfo note;
            if (contains(currentChar)) {
                const int start = pos;
                while (pos < input.length() && contains(input[pos])) {
                    pos++;
                }
                note.lyric() = input.mid(start, pos - start);
                note.language() = id();
                note.category() = category();
            } else {
                const int start = pos;
                while (pos < input.length() && !contains(input[pos])) {
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