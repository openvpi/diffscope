#ifndef ILANGUAGEANALYZER_P_H
#define ILANGUAGEANALYZER_P_H

#include <lyrictool/ilanguageanalyzer.h>

namespace LyricTool {
    
    class ILanguageAnalyzerPrivate {
        Q_DECLARE_PUBLIC(ILanguageAnalyzer)
    public:
        ILanguageAnalyzerPrivate();
        virtual ~ILanguageAnalyzerPrivate();

        void init();

        ILanguageAnalyzer *q_ptr;

        QString id;
        QMDisplayString displayName;
        QMDisplayString description;
    };

}

#endif // ILANGUAGEANALYZER_P_H
