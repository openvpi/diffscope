#ifndef PINYINANALYZER_H
#define PINYINANALYZER_H

#include <QSet>
#include <QObject>

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool::Internal {

    class PinyinAnalyzerPrivate;

    class PinyinAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(PinyinAnalyzer)
    public:
        PinyinAnalyzer(QObject *parent = nullptr);

        void loadDict();

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;

        QString randString() const override;

    private:
        QSet<QString> pinyinSet;
    };

}

#endif // PINYINANALYZER_H
