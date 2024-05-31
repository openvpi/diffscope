#include "linebreakanalyzer.h"

namespace LyricTool::Internal {
    LinebreakAnalyzer::LinebreakAnalyzer(QObject *parent)
        : SingleCharAnalyzer(QStringLiteral("Linebreak"), parent) {
        setDisplayName(tr("Linebreak"));
        setDescription(tr("Capture linebreak."));
    }

    bool LinebreakAnalyzer::contains(QChar c) const {
        return c == QChar::LineFeed || c == QChar::LineSeparator || c == QChar::ParagraphSeparator;
    }

    QString LinebreakAnalyzer::randString() const {
        return {QChar::LineFeed};
    }

}