#ifndef AUDIO_FORMATMANAGER_P_H
#define AUDIO_FORMATMANAGER_P_H

#include <QList>

#include <audioplugin/formatmanager.h>

namespace Audio {

    class FormatManagerPrivate {
        Q_DECLARE_PUBLIC(FormatManager)
    public:
        FormatManager *q_ptr;
        QList<FormatEntry *> entries;
    };

}

#endif // AUDIO_FORMATMANAGER_P_H
