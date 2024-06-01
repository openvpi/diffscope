#include "charsetanalyzer.h"
#include "charsetanalyzer_p.h"

#include <QtCore/QRandomGenerator>

namespace LyricTool {

    CharsetAnalyzerPrivate::CharsetAnalyzerPrivate() {
    }

    void CharsetAnalyzerPrivate::init() {
    }

    CharsetAnalyzer::CharsetAnalyzer(const QString &id, QObject *parent)
        : SingleCharAnalyzer(id, parent) {
    }

    void CharsetAnalyzer::loadDict() {
    }

    bool CharsetAnalyzer::contains(QChar c) const {
        Q_D(const CharsetAnalyzer);
        return d->charset.contains(c);
    }

    QString CharsetAnalyzer::randString() const {
        Q_D(const CharsetAnalyzer);
        if (d->charset.isEmpty()) {
            return {};
        }

        const int randomIndex = QRandomGenerator::global()->bounded(d->charset.size());
        auto it = d->charset.begin();
        std::advance(it, randomIndex);
        return *it;
    }

}