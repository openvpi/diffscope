#ifndef UNKNOWNANALYZER_H
#define UNKNOWNANALYZER_H

#include <QObject>

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool::Internal {

    class UnknownAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
    public:
        UnknownAnalyzer(QObject *parent = nullptr);

        bool contains(const QString &input) const override;

        QList<LyricInfo> split(const QString &input) const override;

        QString randString() const override;
    };

}

#endif // UNKNOWNANALYZER_H
