#ifndef MULTICHARFACTORY_H
#define MULTICHARFACTORY_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class MultiCharAnalyzerPrivate;

    class LYRICTOOL_EXPORT MultiCharAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MultiCharAnalyzer)
    public:
        MultiCharAnalyzer(const QString &id, QObject *parent = nullptr);

        QList<LyricInfo> split(const QString &input) const override;
    };

}

#endif // MULTICHARFACTORY_H
