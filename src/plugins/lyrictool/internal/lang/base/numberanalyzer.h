#ifndef NUMBERANALYZER_H
#define NUMBERANALYZER_H

#include <QObject>

#include <lyrictool/multicharanalyzer.h>

namespace LyricTool::Internal {

    class NumberAnalyzer : public MultiCharAnalyzer {
        Q_OBJECT
    public:
        NumberAnalyzer(QObject *parent = nullptr);

        bool contains(const QString &input) const override;

        QString randString() const override;
    };

}

#endif //NUMBERANALYZER_H
