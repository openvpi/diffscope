#ifndef KANAANALYZER_H
#define KANAANALYZER_H

#include <QObject>

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool::Internal {

    class KanaAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
    public:
        KanaAnalyzer(QObject *parent = nullptr);

        bool contains(const QString &input) const override;
        QList<LyricInfo> split(const QString &input) const override;

        QString randString() const override;
    };

}

#endif // KANAANALYZER_H
