#ifndef ROMAJIANALYZER_H
#define ROMAJIANALYZER_H

#include <QSet>
#include <QObject>

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool::Internal {

    class RomajiAnalyzerPrivate;

    class RomajiAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(RomajiAnalyzer)
    public:
        RomajiAnalyzer(QObject *parent = nullptr);

        void loadDict();

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;

        QString randString() const override;

    private:
        QSet<QString> romajiSet;
    };

}

#endif // ROMAJIANALYZER_H
