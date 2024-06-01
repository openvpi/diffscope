#ifndef ENGLISHANALYZER_H
#define ENGLISHANALYZER_H

#include <QObject>

#include <lyrictool/multicharanalyzer.h>

namespace LyricTool::Internal {

    class EnglishAnalyzer : public MultiCharAnalyzer {
        Q_OBJECT
    public:
        EnglishAnalyzer(QObject *parent = nullptr);

        bool contains(const QString &input) const override;

        QString randString() const override;
    };

}

#endif //ENGLISHANALYZER_H
