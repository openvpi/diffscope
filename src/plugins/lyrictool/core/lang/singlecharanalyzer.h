#ifndef SINGLECHARANALYZER_H
#define SINGLECHARANALYZER_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class LYRICTOOL_EXPORT SingleCharAnalyzer : public LyricTool::ILanguageAnalyzer {
        Q_OBJECT
    public:
        SingleCharAnalyzer(const QString &id, QObject *parent = nullptr);

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;
    };

}

#endif // SINGLECHARANALYZER_H
