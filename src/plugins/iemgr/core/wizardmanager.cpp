#include "wizardmanager.h"

#include <QDebug>

#include <QMCore/qmchronomap.h>

#include <coreplugin/icore.h>

#include "homedialog.h"

namespace IEMgr {

    class WizardManagerPrivate {
        Q_DECLARE_PUBLIC(WizardManager)
    public:
        WizardManagerPrivate();
        virtual ~WizardManagerPrivate();

        void init();

        WizardManager *q_ptr;

        bool running;
        QMChronoMap<QString, IWizard *> importWizards;
    };

    WizardManagerPrivate::WizardManagerPrivate() {
        running = false;
    }

    WizardManagerPrivate::~WizardManagerPrivate() {
    }

    void WizardManagerPrivate::init() {
    }

    static WizardManager *m_instance = nullptr;

    WizardManager *WizardManager::instance() {
        return m_instance;
    }

    bool WizardManager::addWizard(IWizard *factory) {
        Q_D(WizardManager);
        if (!factory) {
            qWarning() << "IEMgr::WizardManager::addWizard(): trying to add null factory";
            return false;
        }
        if (d->importWizards.contains(factory->id())) {
            qWarning() << "IEMgr::WizardManager::addWizard(): trying to add duplicated factory:"
                       << factory->id();
            return false;
        }
        factory->setParent(this);
        d->importWizards.append(factory->id(), factory);
        return true;
    }

    bool WizardManager::removeWizard(IWizard *factory) {
        if (factory == nullptr) {
            qWarning() << "IEMgr::WizardManager::removeWizard(): trying to remove null factory";
            return false;
        }
        return removeWizard(factory->id());
    }

    bool WizardManager::removeWizard(const QString &id) {
        Q_D(WizardManager);
        auto it = d->importWizards.find(id);
        if (it == d->importWizards.end()) {
            qWarning() << "IEMgr::WizardManager::removeWizard(): factory does not exist:" << id;
            return false;
        }
        it.value()->setParent(nullptr);
        d->importWizards.erase(it);
        return true;
    }

    QList<IWizard *> WizardManager::wizards() const {
        Q_D(const WizardManager);
        auto arr = d->importWizards.values();
        return {arr.begin(), arr.end()};
    }

    void WizardManager::clearWizards() {
        Q_D(WizardManager);
        d->importWizards.clear();
    }

    void WizardManager::runImport(QWidget *parent, const QString &wizardId, const QString &path) {
        Q_D(WizardManager);

        auto iWin = Core::ICore::instance()->windowSystem()->findWindow(parent);
        if (!iWin || d->running) {
            return;
        }
        d->running = true;

        Internal::HomeDialog dlg(iWin->window());
        if (!wizardId.isEmpty()) {
            auto wizard = d->importWizards.value(wizardId, nullptr);
            if (wizard)
                dlg.selectWizard(wizard);
        }

        if (!path.isEmpty()) {
            dlg.setCurrentPath(path);
        }

        int code;
        do {
            code = dlg.exec();
        } while (
            code == QDialog::Accepted &&
            !dlg.currentWizard()->runWizard(IWizard::ImportProject, dlg.currentPath(), parent));

        d->running = false;
    }

    void WizardManager::runExport(QWidget *parent, const QString &wizardId, const QString &path) {
        Q_D(WizardManager);

        auto iWin = Core::ICore::instance()->windowSystem()->findWindow(parent);
        if (!iWin || d->running) {
            return;
        }
        d->running = true;

        // ...

        d->running = false;
    }

    bool WizardManager::isRunning() const {
        Q_D(const WizardManager);
        return d->running;
    }

    WizardManager::WizardManager(QObject *parent)
        : WizardManager(*new WizardManagerPrivate(), parent) {
    }

    WizardManager::~WizardManager() {
        m_instance = nullptr;
    }

    WizardManager::WizardManager(WizardManagerPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        m_instance = this;
        d.q_ptr = this;

        d.init();
    }

}
