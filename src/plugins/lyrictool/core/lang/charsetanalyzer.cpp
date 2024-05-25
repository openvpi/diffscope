#include "charsetanalyzer.h"

#include <qrandom.h>

namespace LyricTool {
    CharsetAnalyzer::CharsetAnalyzer(const QString &id, QObject *parent)
        : ILanguageAnalyzer(id, parent) {
    }

    void CharsetAnalyzer::loadDict() {
    }

    bool CharsetAnalyzer::contains(QChar c) const {
        return m_charset.contains(c);
    }

    QString CharsetAnalyzer::randString() const {
        if (m_charset.isEmpty()) {
            return "";
        }

        const int randomIndex = QRandomGenerator::global()->bounded(m_charset.size());
        auto it = m_charset.begin();
        std::advance(it, randomIndex);

        return *it;
    }

}