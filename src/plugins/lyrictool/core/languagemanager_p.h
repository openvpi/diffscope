#ifndef LANGUAGEMANAGERPRIVATE_H
#define LANGUAGEMANAGERPRIVATE_H

#include <QMCore/qmchronomap.h>

#include <lyrictool/languagemanager.h>

namespace LyricTool {

    class LanguageManagerPrivate {
        Q_DECLARE_PUBLIC(LanguageManager)
    public:
        LanguageManagerPrivate();
        virtual ~LanguageManagerPrivate();

        void init();

        LanguageManager *q_ptr;

        QMChronoMap<QString, ILanguageAnalyzer *> languages;
        QMChronoMap<QString, IG2pConverter *> g2ps;
    };

}

#endif // LANGUAGEMANAGERPRIVATE_H