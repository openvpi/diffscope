#ifndef CHARSETANALYZER_H
#define CHARSETANALYZER_H

#include <QSet>
#include <lyrictool/singlecharanalyzer.h>

namespace LyricTool {

    class CharsetAnalyzerPrivate;

    class LYRICTOOL_EXPORT CharsetAnalyzer : public SingleCharAnalyzer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CharsetAnalyzer)
    public:
        CharsetAnalyzer(const QString &id, QObject *parent = nullptr);

        virtual void loadDict();

        bool contains(QChar c) const override;

        QString randString() const override;

    protected:
        CharsetAnalyzer(CharsetAnalyzerPrivate &d, const QString &id, QObject *parent = nullptr);
    };

}

#endif // CHARSETANALYZER_H
