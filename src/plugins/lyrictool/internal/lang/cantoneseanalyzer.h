#ifndef CANTONESEANALYZER_H
#define CANTONESEANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class CantoneseAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        CantoneseAnalyzer(QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif //CANTONESEANALYZER_H
