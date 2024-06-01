#include "punctuationanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {
    PunctuationAnalyzer::PunctuationAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Punctuation"), parent) {
        setDisplayName(tr("Punctuation"));
        setDescription(tr("Capture punctuations."));
        setDiscardResult(true);
    }

    bool PunctuationAnalyzer::contains(QChar c) const {
        return c.isPunct() && c != '-';
    }

    QString PunctuationAnalyzer::randString() const {
        QChar randomPunctuation;
        do {
            const int codePoint = QRandomGenerator::global()->bounded(0x2000, 0x206F + 1);
            randomPunctuation = QChar(codePoint);
        } while (randomPunctuation == QChar('-'));

        return randomPunctuation;
    }

}