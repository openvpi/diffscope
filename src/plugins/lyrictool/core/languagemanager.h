#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QtCore/QObject>

#include <lyrictool/ilanguageanalyzer.h>
#include <lyrictool/ig2pconverter.h>

namespace LyricTool {

    class LanguageManagerPrivate;

    class LYRICTOOL_EXPORT LanguageManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(LanguageManager)
    public:
        explicit LanguageManager(QObject *parent = nullptr);
        ~LanguageManager();

    public:
        bool load(QString *errorMessage);

        ILanguageAnalyzer *language(const QString &id) const;
        QList<ILanguageAnalyzer *> languages() const;
        bool addLanguage(ILanguageAnalyzer *factory);
        bool removeLanguage(const ILanguageAnalyzer *factory);
        bool removeLanguage(const QString &id);
        void clearLanguages();

        IG2pConverter *g2p(const QString &id) const;
        QList<IG2pConverter *> g2ps() const;
        bool addG2p(ILanguageAnalyzer *factory);
        bool removeG2p(const ILanguageAnalyzer *factory);
        bool removeG2p(const QString &id);
        void clearG2ps();

    public:
        QStringList sortedLanguages() const;
        void setSortedLanguages(const QString &sortedLanguages);

        QList<LyricInfo> split(const QString &input) const;
        QList<LyricInfo> correct(const QList<LyricInfo> &input) const;
        QList<LyricInfo> convert(const QList<LyricInfo> &input) const;

        QString analysis(const QString &input) const;
        QStringList analysis(const QStringList &input) const;

    protected:
        LanguageManager(LanguageManagerPrivate &d, QObject *parent = nullptr);

        QScopedPointer<LanguageManagerPrivate> d_ptr;
    };

}

#endif // LANGUAGEMANAGER_H