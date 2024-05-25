#ifndef MULTICHARFACTORY_H
#define MULTICHARFACTORY_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class LYRICTOOL_EXPORT MultiCharAnalyzer : public LyricTool::ILanguageAnalyzer {
        Q_OBJECT
    public:
        MultiCharAnalyzer(const QString &id, QObject *parent = nullptr);

        QList<LyricInfo> split(const QString &input) const override;
    };

}

#endif // MULTICHARFACTORY_H
