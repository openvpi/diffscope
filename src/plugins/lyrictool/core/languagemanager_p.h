#ifndef LANGUAGEMANAGERPRIVATE_H
#define LANGUAGEMANAGERPRIVATE_H

#include "LanguageManager.h"

namespace LyricTool {

class LanguageManagerPrivate {
    Q_DECLARE_PUBLIC(LanguageManager)
public:
    LanguageManagerPrivate();
    virtual ~LanguageManagerPrivate();

    void init();

    LanguageManager *q_ptr;
};

}

#endif // LANGUAGEMANAGERPRIVATE_H