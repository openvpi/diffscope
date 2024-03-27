#ifndef WIZARDMANAGER_H
#define WIZARDMANAGER_H

#include <QObject>

#include <iemgr/iwizard.h>

namespace IEMgr {

    namespace Internal {
        class IEMgrPlugin;
    }

    class WizardManagerPrivate;

    class IEMGR_EXPORT WizardManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WizardManager)
    public:
        static WizardManager *instance();

    public:
        bool addWizard(IWizard *factory);
        bool removeWizard(IWizard *factory);
        bool removeWizard(const QString &id);
        QList<IWizard *> wizards() const;
        void clearWizards();

    public:
        void runImport(QWidget *parent, const QString &wizardId = {}, const QString &path = {});
        void runExport(QWidget *parent, const QString &wizardId = {}, const QString &path = {});

        bool isRunning() const;

    private:
        explicit WizardManager(QObject *parent = nullptr);
        ~WizardManager();

        WizardManager(WizardManagerPrivate &d, QObject *parent = nullptr);

        QScopedPointer<WizardManagerPrivate> d_ptr;

        friend class Internal::IEMgrPlugin;
    };

}

#endif // WIZARDMANAGER_H