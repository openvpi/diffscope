#include "ilanguageanalyzer.h"
#include "ilanguageanalyzer_p.h"

namespace LyricTool {

    ILanguageAnalyzerPrivate::ILanguageAnalyzerPrivate() {
    }

    ILanguageAnalyzerPrivate::~ILanguageAnalyzerPrivate() {
    }

    void ILanguageAnalyzerPrivate::init() {
    }

    ILanguageAnalyzer::ILanguageAnalyzer(const QString &id, QObject *parent)
        : ILanguageAnalyzer(*new ILanguageAnalyzerPrivate(), id, parent) {
    }

    ILanguageAnalyzer::~ILanguageAnalyzer() = default;

    void ILanguageAnalyzer::correct(const QList<LyricInfo> &input) const {
        for (const auto &note : input) {
            if (note.language() == QStringLiteral("Unknown")) {
                if (contains(note.lyric()))
                    note.language() = id();
            }
        }
    }

    QString ILanguageAnalyzer::analyze(const QString &input) const {
        return contains(input) ? id() : QStringLiteral("Unknown");
    }

    QList<LyricInfo> ILanguageAnalyzer::split(const QList<LyricInfo> &input) const {
        Q_D(const ILanguageAnalyzer);
        if (!d->enabled) {
            return input;
        }

        QList<LyricInfo> result;
        for (const auto &note : input) {
            if (note.language() == QStringLiteral("Unknown")) {
                const auto splitRes = split(note.lyric());
                for (const auto &res : splitRes) {
                    if (res.language() == id() && d->discardResult) {
                        continue;
                    }
                    result.append(res);
                }
            } else {
                result.append(note);
            }
        }
        return result;
    }

    QList<LyricInfo> ILanguageAnalyzer::split(const QString &input) const {
        Q_UNUSED(input);
        return {};
    }

    QString ILanguageAnalyzer::randString() const {
        return {};
    }

    bool ILanguageAnalyzer::contains(const QString &input) const {
        Q_UNUSED(input);
        return false;
    }

    bool ILanguageAnalyzer::contains(QChar c) const {
        Q_UNUSED(c);
        return false;
    }

    QString ILanguageAnalyzer::id() const {
        Q_D(const ILanguageAnalyzer);
        return d->id;
    }

    QVariantMap ILanguageAnalyzer::g2pConfig() {
        Q_D(const ILanguageAnalyzer);
        return d->m_g2pConfig;
    }

    QString ILanguageAnalyzer::displayName() const {
        Q_D(const ILanguageAnalyzer);
        return d->displayName;
    }

    void ILanguageAnalyzer::setDisplayName(const QMDisplayString &displayName) {
        Q_D(ILanguageAnalyzer);
        d->displayName = displayName;
    }

    QString ILanguageAnalyzer::description() const {
        Q_D(const ILanguageAnalyzer);
        return d->description;
    }

    void ILanguageAnalyzer::setDescription(const QMDisplayString &description) {
        Q_D(ILanguageAnalyzer);
        d->description = description;
    }

    QString ILanguageAnalyzer::category() const {
        Q_D(const ILanguageAnalyzer);
        return d->categroy;
    }

    void ILanguageAnalyzer::setCategory(const QString &category) {
        Q_D(ILanguageAnalyzer);
        d->categroy = category;
    }

    QString ILanguageAnalyzer::selectedG2p() const {
        Q_D(const ILanguageAnalyzer);
        return d->m_selectedG2p;
    }

    void ILanguageAnalyzer::setG2p(const QString &g2pId) {
        Q_D(ILanguageAnalyzer);
        d->m_selectedG2p = g2pId;
    }

    bool ILanguageAnalyzer::enabled() const {
        Q_D(const ILanguageAnalyzer);
        return d->enabled;
    }

    void ILanguageAnalyzer::setEnabled(const bool &enable) {
        Q_D(ILanguageAnalyzer);
        d->enabled = enable;
    }

    bool ILanguageAnalyzer::discardResult() const {
        Q_D(const ILanguageAnalyzer);
        return d->discardResult;
    }

    void ILanguageAnalyzer::setDiscardResult(const bool &discard) {
        Q_D(ILanguageAnalyzer);
        d->discardResult = discard;
    }

    ILanguageAnalyzer::ILanguageAnalyzer(ILanguageAnalyzerPrivate &d, const QString &id,
                                         QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.categroy = id;
        d.m_selectedG2p = id;

        d.init();
    }

}
