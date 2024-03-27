#ifndef ICORE_H
#define ICORE_H

#include <QObject>
#include <QSettings>

#include <CoreApi/icorebase.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class ICorePrivate;

    class CORE_EXPORT ICore : public ICoreBase {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ICore)
    public:
        static ICore *instance();

        static QString mainTitle();
        static QString displayTitle(const QString &text);
        static void aboutApp(QWidget *parent = nullptr);
        static void fatalError(QWidget *parent, const QString &text, int exitCode = -1);

    public:
        int showSettingsDialog(const QString &id, QWidget *parent);
        void showHome();

        void newFile() const;
        bool openFile(const QString &fileName, QWidget *parent = nullptr) const;

    private:
        explicit ICore(QObject *parent = nullptr);
        ~ICore();

        ICore(ICorePrivate &d, QObject *parent = nullptr);

        friend class Internal::CorePlugin;
    };

}

#endif // ICORE_H
