#include "mandarinanalyzer.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool::Internal {

    MandarinAnalyzer::MandarinAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Mandarin"), parent) {
        setDisplayName(tr("Mandarin"));
        setDescription(tr("Capture Mandarin characters."));
    }

    static bool isHanzi(QChar c) {
        return c >= QChar(0x4e00) && c <= QChar(0x9fa5);
    }

    bool MandarinAnalyzer::contains(QChar c) const {
        return isHanzi(c);
    }

    QString MandarinAnalyzer::randString() const {
        const int unicode = QRandomGenerator::global()->bounded(0x4e00, 0x9fa5 + 1);
        return {QChar(unicode)};
    }

}