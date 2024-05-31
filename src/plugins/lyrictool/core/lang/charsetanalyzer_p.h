#ifndef CHARSETANALYZER_P_H
#define CHARSETANALYZER_P_H

#include <QtCore/QSet>

#include <lyrictool/charsetanalyzer.h>
#include <lyrictool/private/ilanguageanalyzer_p.h>

namespace LyricTool {

    class CharsetAnalyzerPrivate : public ILanguageAnalyzerPrivate {
        Q_DECLARE_PUBLIC(CharsetAnalyzer)
    public:
        CharsetAnalyzerPrivate();

        void init();

        QSet<QChar> charset;
    };

}

#endif // CHARSETANALYZER_P_H
