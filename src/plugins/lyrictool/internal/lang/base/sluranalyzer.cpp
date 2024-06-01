#include "sluranalyzer.h"

namespace LyricTool::Internal {
    SlurAnalyzer::SlurAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Slur"), parent) {
        setDisplayName(tr("Slur"));
        setDescription(tr("Capture slur."));
    }

    bool SlurAnalyzer::contains(QChar c) const {
        return c == '-';
    }

    QString SlurAnalyzer::randString() const {
        return "-";
    }

}