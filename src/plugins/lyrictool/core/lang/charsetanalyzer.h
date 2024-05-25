#ifndef CHARSETANALYZER_H
#define CHARSETANALYZER_H

#include <QSet>
#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {

    class LYRICTOOL_EXPORT CharsetAnalyzer : public LyricTool::ILanguageAnalyzer {
        Q_OBJECT
    public:
        CharsetAnalyzer(const QString &id, QObject *parent = nullptr);

        virtual void loadDict();

        bool contains(QChar c) const override;

        QString randString() const override;

    protected:
        QSet<QChar> m_charset;
    };

}

#endif // CHARSETANALYZER_H
