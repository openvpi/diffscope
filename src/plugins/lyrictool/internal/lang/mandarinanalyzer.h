#ifndef MANDARINANALYZER_H
#define MANDARINANALYZER_H

#include <QObject>

#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool::Internal {

    class MandarinAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
    public:
        MandarinAnalyzer(QObject *parent = nullptr);

        bool contains(QChar c) const override;

        QString randString() const override;
    };

}

#endif // MANDARINANALYZER_H
