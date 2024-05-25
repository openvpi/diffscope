#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QtCore/QObject>

#include <lyrictool/lyrictoolglobal.h>

namespace LyricTool {

    class LanguageManagerPrivate;

    class LYRICTOOL_EXPORT LanguageManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(LanguageManager)
    public:
        explicit LanguageManager(QObject *parent = nullptr);
        ~LanguageManager();

    protected:
        LanguageManager(LanguageManagerPrivate &d, QObject *parent = nullptr);

        QScopedPointer<LanguageManagerPrivate> d_ptr;
    };

}

#endif // LANGUAGEMANAGER_H