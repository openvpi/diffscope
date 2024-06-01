#ifndef MULTICHARANALYZER_P_H
#define MULTICHARANALYZER_P_H

#include <lyrictool/multicharanalyzer.h>
#include <lyrictool/private/ilanguageanalyzer_p.h>

namespace LyricTool {

    class MultiCharAnalyzerPrivate : public ILanguageAnalyzerPrivate {
        Q_DECLARE_PUBLIC(MultiCharAnalyzer)
    public:
        MultiCharAnalyzerPrivate();

        void init();
    };

}

#endif // MULTICHARANALYZER_P_H
