#ifndef PUNCTUATIONANALYZER_H
#define PUNCTUATIONANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class PunctuationAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        PunctuationAnalyzer(QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif //PUNCTUATIONANALYZER_H
