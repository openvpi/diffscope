#include "englishanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {
    EnglishAnalyzer::EnglishAnalyzer(QObject *parent)
        : MultiCharAnalyzer(QStringLiteral("English"), parent) {
        setDisplayName(tr("English"));
        setDescription(tr("Capture English words."));
        setDiscardResult(true);
    }

    bool isLetter(QChar c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\'';
    }

    bool EnglishAnalyzer::contains(const QString &input) const {
        for (const QChar &ch : input) {
            if (!isLetter(ch)) {
                return false;
            }
        }
        return true;
    }

    QString EnglishAnalyzer::randString() const {
        QString word;
        const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
        const int length = QRandomGenerator::global()->bounded(1, 12);

        for (int i = 0; i < length; ++i) {
            const int index = QRandomGenerator::global()->bounded(alphabet.length());
            word.append(alphabet.at(index));
        }

        return word;
    }

}