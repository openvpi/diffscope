#include "ilanguageanalyzer.h"
#include "ilanguageanalyzer_p.h"

namespace LyricTool {

    ILanguageAnalyzerPrivate::ILanguageAnalyzerPrivate() {
    }

    ILanguageAnalyzerPrivate::~ILanguageAnalyzerPrivate() {
    }

    void ILanguageAnalyzerPrivate::init() {
    }

    ILanguageAnalyzer::~ILanguageAnalyzer() = default;

    void ILanguageAnalyzer::correct(const QList<LyricInfo> &input) const {
    }

    QString ILanguageAnalyzer::analyze(const QString &input) const {
        return QString();
    }

    QList<LyricInfo> ILanguageAnalyzer::split(const QList<LyricInfo> &input) const {
        return QList<LyricInfo>();
    }

    QList<LyricInfo> ILanguageAnalyzer::split(const QString &input) const {
        return QList<LyricInfo>();
    }

    QString ILanguageAnalyzer::randString() const {
        return QString();
    }

    bool ILanguageAnalyzer::contains(const QString &input) const {
        return false;
    }

    bool ILanguageAnalyzer::contains(QChar c) const {
        return false;
    }

    QString ILanguageAnalyzer::id() const {
        Q_D(const ILanguageAnalyzer);
        return d->id;
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

    ILanguageAnalyzer::ILanguageAnalyzer(ILanguageAnalyzerPrivate &d, const QString &id,
                                         QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
