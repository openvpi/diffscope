#include "debugplugin.h"

#include <QMessageBox>
#include <QApplication>
#include <QtTest/QtTest>

#include <extensionsystem/pluginspec.h>

#include <QMWidgets/qmdecoratorv2.h>

#include <CoreApi/iloader.h>

#include <idecoreFramework/initroutine.h>

#include <coreplugin/icore.h>

#include <debugplugin/internal/debugpage.h>

namespace Debug::Internal {

#ifdef DEBUG_TEST

    class DebugPluginInteractiveDebug : public QObject {
        Q_OBJECT
    public:
        explicit DebugPluginInteractiveDebug(QObject *parent = nullptr) : QObject(parent) {
        }
    private slots:
        void func1() {
            QMessageBox::information(nullptr, {}, "Test 1");
        }
        void func2() {
            QMessageBox::information(nullptr, {}, "Test 2");
        }
    };

    class DebugPluginUnitTest : public QObject {
        Q_OBJECT
    public:
        explicit DebugPluginUnitTest(QObject *parent = nullptr) : QObject(parent) {
        }
    private slots:
        void func() {
            QVERIFY(true);
        }
    };

#endif

    DebugPlugin::DebugPlugin() {
    }

    DebugPlugin::~DebugPlugin() = default;

    bool DebugPlugin::initialize(const QStringList &arguments, QString *errorString) {
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));

        qDebug() << "Debug::DebugPlugin: initializing";
        auto splash = Core::InitRoutine::splash();
        splash->showMessage(tr("Initializing debug plugin..."));

        auto sc = Core::ICore::instance()->settingCatalog();
        sc->addPage(new DebugPage);

#ifdef DEBUG_TEST
      auto iLoader = Core::ILoader::instance();
      iLoader->addObject("debug.InteractiveDebug", new DebugPluginInteractiveDebug(this));
      iLoader->addObject("debug.UnitTest", new DebugPluginUnitTest(this));
#endif

        return true;
    }
    void DebugPlugin::extensionsInitialized() {


    }
    bool DebugPlugin::delayedInitialize() {
        return false;
    }


}

#ifdef DEBUG_TEST
#include "debugplugin.moc"
#endif