#ifndef SLURANALYZER_H
#define SLURANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class SlurAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        SlurAnalyzer(QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif //SLURANALYZER_H
