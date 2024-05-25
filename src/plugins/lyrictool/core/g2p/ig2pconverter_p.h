#ifndef IG2PPRIVATE_H
#define IG2PPRIVATE_H

#include <lyrictool/ig2pconverter.h>

namespace LyricTool {

    class IG2pConverterPrivate {
        Q_DECLARE_PUBLIC(IG2pConverter)
    public:
        IG2pConverterPrivate();
        virtual ~IG2pConverterPrivate();

        void init();

        IG2pConverter *q_ptr;

        QString id;
        QMDisplayString displayName;
        QMDisplayString description;
    };

}

#endif // IG2PPRIVATE_H