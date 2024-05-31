#include "languagemanager.h"
#include "languagemanager_p.h"

namespace LyricTool {

    LanguageManagerPrivate::LanguageManagerPrivate() {
    }

    LanguageManagerPrivate::~LanguageManagerPrivate() {
    }

    void LanguageManagerPrivate::init() {
    }

    LanguageManager::LanguageManager(QObject *parent)
        : LanguageManager(*new LanguageManagerPrivate(), parent) {
    }

    LanguageManager::~LanguageManager() {
    }

    bool LanguageManager::load(QString *errorMessage) {
        return false;
    }
    ILanguageAnalyzer *LanguageManager::language(const QString &id) const {
        return nullptr;
    }
    QList<ILanguageAnalyzer *> LanguageManager::languages() const {
        return {};
    }
    bool LanguageManager::addLanguage(ILanguageAnalyzer *factory) {
        return false;
    }
    bool LanguageManager::removeLanguage(const ILanguageAnalyzer *factory) {
        return false;
    }
    bool LanguageManager::removeLanguage(const QString &id) {
        return false;
    }
    void LanguageManager::clearLanguages() {
    }
    IG2pConverter *LanguageManager::g2p(const QString &id) const {
        return nullptr;
    }
    QList<IG2pConverter *> LanguageManager::g2ps() const {
        return {};
    }
    bool LanguageManager::addG2p(ILanguageAnalyzer *factory) {
        return false;
    }
    bool LanguageManager::removeG2p(const ILanguageAnalyzer *factory) {
        return false;
    }
    bool LanguageManager::removeG2p(const QString &id) {
        return false;
    }
    void LanguageManager::clearG2ps() {
    }
    QStringList LanguageManager::sortedLanguages() const {
        return QStringList();
    }
    void LanguageManager::setSortedLanguages(const QString &sortedLanguages) {
    }
    QList<LyricInfo> LanguageManager::split(const QString &input) const {
        return QList<LyricInfo>();
    }
    QList<LyricInfo> LanguageManager::correct(const QList<LyricInfo> &input) const {
        return QList<LyricInfo>();
    }
    QList<LyricInfo> LanguageManager::convert(const QList<LyricInfo> &input) const {
        return QList<LyricInfo>();
    }
    QString LanguageManager::analysis(const QString &input) const {
        return QString();
    }
    QStringList LanguageManager::analysis(const QStringList &input) const {
        return QStringList();
    }

    LanguageManager::LanguageManager(LanguageManagerPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
