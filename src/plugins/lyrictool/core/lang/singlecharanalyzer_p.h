#ifndef SINGLECHARANALYZER_P_H
#define SINGLECHARANALYZER_P_H

#include <lyrictool/singlecharanalyzer.h>
#include <lyrictool/private/ilanguageanalyzer_p.h>

namespace LyricTool {

    class SingleCharAnalyzerPrivate : public ILanguageAnalyzerPrivate {
        Q_DECLARE_PUBLIC(SingleCharAnalyzer)
    public:
        SingleCharAnalyzerPrivate();

        void init();
    };

}

#endif // SINGLECHARANALYZER_P_H
