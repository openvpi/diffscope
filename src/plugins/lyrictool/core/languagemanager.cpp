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
        Q_D(const LanguageManager);
        return d->languages.value(id);
    }

    QList<ILanguageAnalyzer *> LanguageManager::languages() const {
        Q_D(const LanguageManager);
        return d->languages.values_qlist();
    }

    bool LanguageManager::addLanguage(ILanguageAnalyzer *analyzer) {
        Q_D(LanguageManager);
        if (!analyzer) {
            qWarning() << "LyricTool::LanguageManager::addLanguage(): trying to add null analyzer";
            return false;
        }
        if (d->languages.contains(analyzer->id())) {
            qWarning()
                << "LyricTool::LanguageManager::addLanguage(): trying to add duplicated analyzer:"
                << analyzer->id();
            return false;
        }
        analyzer->setParent(this);
        d->languages.append(analyzer->id(), analyzer);
        return true;
    }

    bool LanguageManager::removeLanguage(const ILanguageAnalyzer *analyzer) {
        if (analyzer == nullptr) {
            qWarning()
                << "LyricTool::LanguageManager::removeLanguage(): trying to remove null analyzer";
            return false;
        }
        return removeLanguage(analyzer->id());
    }

    bool LanguageManager::removeLanguage(const QString &id) {
        Q_D(LanguageManager);
        const auto it = d->languages.find(id);
        if (it == d->languages.end()) {
            qWarning() << "LyricTool::LanguageManager::removeLanguage(): analyzer does not exist:"
                       << id;
            return false;
        }
        it.value()->setParent(nullptr);
        d->languages.erase(it);
        return true;
    }

    void LanguageManager::clearLanguages() {
        Q_D(LanguageManager);
        d->languages.clear();
    }

    IG2pConverter *LanguageManager::g2p(const QString &id) const {
        Q_D(const LanguageManager);
        return d->g2ps.value(id);
    }

    QList<IG2pConverter *> LanguageManager::g2ps() const {
        Q_D(const LanguageManager);
        return d->g2ps.values_qlist();
    }

    bool LanguageManager::addG2p(IG2pConverter *converter) {
        Q_D(LanguageManager);
        if (!converter) {
            qWarning() << "LyricTool::LanguageManager::addG2p(): trying to add null converter";
            return false;
        }
        if (d->g2ps.contains(converter->id())) {
            qWarning()
                << "LyricTool::LanguageManager::addG2p(): trying to add duplicated converter:"
                << converter->id();
            return false;
        }
        converter->setParent(this);
        d->g2ps.append(converter->id(), converter);
        return true;
    }

    bool LanguageManager::removeG2p(const IG2pConverter *converter) {
        if (converter == nullptr) {
            qWarning()
                << "LyricTool::LanguageManager::removeG2p(): trying to remove null converter";
            return false;
        }
        return removeG2p(converter->id());
    }

    bool LanguageManager::removeG2p(const QString &id) {
        Q_D(LanguageManager);
        const auto it = d->g2ps.find(id);
        if (it == d->g2ps.end()) {
            qWarning() << "LyricTool::LanguageManager::removeG2p(): converter does not exist:"
                       << id;
            return false;
        }
        it.value()->setParent(nullptr);
        d->g2ps.erase(it);
        return true;
    }

    void LanguageManager::clearG2ps() {
        Q_D(LanguageManager);
        d->g2ps.clear();
    }

    QStringList LanguageManager::sortedLanguages() const {
        Q_D(const LanguageManager);
        return d->defaultSort;
    }

    void LanguageManager::setSortedLanguages(const QStringList &sortedLanguages) {
        Q_D(LanguageManager);
        d->defaultSort = sortedLanguages;
    }

    QList<ILanguageAnalyzer *>
        LanguageManager::priorityLanguages(const QStringList &priorityList) const {
        Q_D(const LanguageManager);
        QStringList order = this->sortedLanguages();

        QList<ILanguageAnalyzer *> result;
        for (const auto &category : priorityList) {
            for (const auto &lang : order) {
                const auto analyzer = language(lang);
                if (analyzer != nullptr && analyzer->category() == category &&
                    !result.contains(analyzer)) {
                    result.append(analyzer);
                }
            }
        }

        for (const auto &id : order) {
            const auto analyzer = language(id);
            if (analyzer != nullptr && !result.contains(analyzer)) {
                result.append(analyzer);
            }
        }
        return result;
    }

    QList<LyricInfo> LanguageManager::split(const QString &input) const {
        const auto analyzers = this->priorityLanguages();
        LyricInfo lyricInfo;
        lyricInfo.setLyric(input);
        QList<LyricInfo> result = {lyricInfo};

        for (const auto &analyzer : analyzers) {
            if (analyzer != nullptr)
                result = analyzer->split(result);
        }
        return result;
    }

    QList<LyricInfo> LanguageManager::correct(const QList<LyricInfo> &input) const {
        QList<LyricInfo> result = input;
        const auto analyzers = this->priorityLanguages();
        for (const auto &analyzer : analyzers) {
            if (analyzer != nullptr)
                analyzer->correct(result);
        }
        return result;
    }

    QList<LyricInfo> LanguageManager::convert(const QList<LyricInfo> &input) const {
        QList<LyricInfo> result = input;

        QMap<QString, QList<int>> languageIndexMap;
        QMap<QString, QStringList> languageLyricMap;

        for (int i = 0; i < input.size(); ++i) {
            const LyricInfo &info = input.at(i);
            languageIndexMap[info.language()].append(i);
            languageLyricMap[info.language()].append(info.lyric());
        }

        const auto languages = languageIndexMap.keys();
        for (const auto &language : languages) {
            const auto rawLyrics = languageLyricMap[language];
            auto g2pConverter = this->g2p(this->language(language)->selectedG2p());
            if (g2pConverter == nullptr)
                g2pConverter = this->g2p(QStringLiteral("Unknown"));
            const auto analyzer = this->language(language);
            const auto g2pConfig = analyzer != nullptr ? analyzer->g2pConfig() : QVariantMap();

            const auto tempRes = g2pConverter->convert(rawLyrics, g2pConfig);
            for (int i = 0; i < tempRes.size(); i++) {
                const auto index = languageIndexMap[language][i];
                result[index].setSyllable(tempRes[i].syllable());
                result[index].setError(tempRes[i].error());
                result[index].setCandidates(tempRes[i].candidates());
            }
        }
        return result;
    }

    QString LanguageManager::analysis(const QString &input) const {
        QString result = QStringLiteral("Unknown");
        auto analyzers = this->priorityLanguages();

        for (const auto &analyzer : analyzers) {
            if (analyzer != nullptr)
                result = analyzer->analyze(input);
            if (result != QStringLiteral("Unknown"))
                break;
        }

        return result;
    }

    QStringList LanguageManager::analysis(const QStringList &input) const {
        auto analyzers = this->priorityLanguages();
        QList<LyricInfo> rawInfo;
        for (const auto &lyric : input) {
            LyricInfo lyricInfo;
            lyricInfo.setLyric(lyric);
            rawInfo.append(lyricInfo);
        }

        for (const auto &analyzer : analyzers) {
            if (analyzer != nullptr)
                analyzer->correct(rawInfo);
        }

        QStringList result;
        for (const auto &info : rawInfo)
            result.append(info.language());
        return result;
    }

    LanguageManager::LanguageManager(LanguageManagerPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
