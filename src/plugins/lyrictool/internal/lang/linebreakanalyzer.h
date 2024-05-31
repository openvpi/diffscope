#ifndef LINEBREAKANALYZER_H
#define LINEBREAKANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class LinebreakAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        LinebreakAnalyzer(const QString &id = "Linebreak", QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif // LINEBREAKANALYZER_H
