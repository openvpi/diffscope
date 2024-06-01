#ifndef DICTANALYZER_H
#define DICTANALYZER_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class DictAnalyzerPrivate;

    class LYRICTOOL_EXPORT DictAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DictAnalyzer)
    public:
        DictAnalyzer(const QString &id, QObject *parent = nullptr);

        virtual void loadDict();

        bool contains(QChar c) const override;

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;

        QString randString() const override;

    protected:
        DictAnalyzer(DictAnalyzerPrivate &d, const QString &id, QObject *parent = nullptr);
    };

}

#endif // DICTANALYZER_H
