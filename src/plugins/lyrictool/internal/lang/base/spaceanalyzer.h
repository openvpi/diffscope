#ifndef SPACEANALYZER_H
#define SPACEANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class SpaceAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        SpaceAnalyzer(QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif // SPACEANALYZER_H
