#include "singlecharanalyzer.h"
#include "singlecharanalyzer_p.h"

namespace LyricTool {
    SingleCharAnalyzerPrivate::SingleCharAnalyzerPrivate() {
    }

    void SingleCharAnalyzerPrivate::init() {
    }

    SingleCharAnalyzer::SingleCharAnalyzer(const QString &id, QObject *parent)
        : ILanguageAnalyzer(id, parent) {
    }

    bool SingleCharAnalyzer::contains(const QString &input) const {
        if (input.size() == 1)
            return contains(input.at(0));
        return false;
    }

    QList<LyricInfo> SingleCharAnalyzer::split(const QString &input) const {
        QList<LyricInfo> result;

        int pos = 0;
        while (pos < input.length()) {
            const QChar &currentChar = input[pos];
            LyricInfo note;

            if (contains(currentChar)) {
                note.lyric() = input.mid(pos, 1);
                note.language() = id();
                note.category() = category();
                pos++;
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