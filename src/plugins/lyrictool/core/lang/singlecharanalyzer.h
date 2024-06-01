#ifndef SINGLECHARANALYZER_H
#define SINGLECHARANALYZER_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class SingleCharAnalyzerPrivate;

    class LYRICTOOL_EXPORT SingleCharAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SingleCharAnalyzer)
    public:
        SingleCharAnalyzer(const QString &id, QObject *parent = nullptr);

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;
    };

}

#endif // SINGLECHARANALYZER_H
