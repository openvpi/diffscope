#include "cantoneseanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {

    CantoneseAnalyzer::CantoneseAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Cantonese"), parent) {
        setDisplayName(tr("Cantonese"));
        setDescription(tr("Capture Cantonese characters."));
    }

    static bool isHanzi(QChar c) {
        return c >= QChar(0x4e00) && c <= QChar(0x9fa5);
    }

    bool CantoneseAnalyzer::contains(QChar c) const {
        return isHanzi(c);
    }

    QString CantoneseAnalyzer::randString() const {
        const int unicode = QRandomGenerator::global()->bounded(0x4e00, 0x9fa5 + 1);
        return {QChar(unicode)};
    }

}