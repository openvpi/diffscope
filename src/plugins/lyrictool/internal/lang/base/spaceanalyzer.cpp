#include "spaceanalyzer.h"

namespace LyricTool::Internal {
    SpaceAnalyzer::SpaceAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Space"), parent) {
        setDisplayName(tr("Space"));
        setDescription(tr("Capture space."));
    }

    bool SpaceAnalyzer::contains(QChar c) const {
        return c == ' ';
    }

    QString SpaceAnalyzer::randString() const {
        return " ";
    }

}