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
        QString categroy;
        QMDisplayString displayName;
        QMDisplayString description;

        bool enabled = true;
        bool discardResult = false;

        QString m_selectedG2p;
        QVariantMap m_g2pConfig;
    };

}

#endif // ILANGUAGEANALYZER_P_H
