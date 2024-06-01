#include "numberanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {
    NumberAnalyzer::NumberAnalyzer(QObject *parent)
        : MultiCharAnalyzer(QStringLiteral("Number"), parent) {
        setDisplayName(tr("Number"));
        setDescription(tr("Capture numbers."));
        setDiscardResult(true);
    }

    bool isNumber(QChar c) {
        return c.isDigit();
    }

    bool NumberAnalyzer::contains(const QString &input) const {
        for (const QChar &ch : input) {
            if (!isNumber(ch)) {
                return false;
            }
        }
        return true;
    }

    QString NumberAnalyzer::randString() const {
        QString word;
        const QString alphabet = "0123456789";
        const int length = QRandomGenerator::global()->bounded(1, 5);

        for (int i = 0; i < length; ++i) {
            const int index = QRandomGenerator::global()->bounded(alphabet.length());
            word.append(alphabet.at(index));
        }

        return word;
    }

}