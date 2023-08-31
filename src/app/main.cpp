#include <QApplication>
#include <QDebug>
#include <QNetworkProxyFactory>

#include <extensionsystem/pluginmanager.h>

#include <QBreakpadHandler.h>

#include <QMAppExtension.h>
#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <CoreApi/ILoader.h>

#include <loaderconfig.h>

#include "choruskit_config.h"

static const char APP_PLUGIN_IID[] = "org.ChorusKit." APP_NAME ".Plugin";

class MyLoaderConfiguration : public LoaderConfiguration {
public:
    MyLoaderConfiguration() {
        allowRoot = false;
        pluginIID = APP_PLUGIN_IID;
        splashSettingPath = qAppExt->appShareDir() + "/config.json";
        userSettingsPath = qAppExt->appDataDir();
        systemSettingsPath = qAppExt->appShareDir();

        pluginPaths << qAppExt->appPluginsDir();
    }

    bool preprocessArguments(QStringList &arguments, int *code) override {
        // Skip if show help
        const auto &realArgs = QApplication::arguments();
        const auto &realArgsSet = QSet<QString>(realArgs.begin(), realArgs.end());
        if (realArgsSet.contains("-h") || realArgsSet.contains("--help")) {
            return true;
        }

        // Create app data path and temp path
        if (!qAppExt->createDataAndTempDirs()) {
            *code = -1;
            return false;
        }

        return true;
    }

    void beforeLoadPlugins(QSplashScreen *screen) override {
        // Set global settings path
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, userSettingsPath);
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, systemSettingsPath);

        // Make sure we honor the system's proxy settings
        QNetworkProxyFactory::setUseSystemConfiguration(true);

        // Don't show plugin manager debug info
        QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

        // Restore language and themes
        {
            auto settings = ExtensionSystem::PluginManager::settings();
            settings->beginGroup("Preferences");
            auto value = settings->value("Translation");
            if (value.type() == QVariant::String) {
                qIDec->setLocale(value.toString());
            }

            value = settings->value("Theme");
            if (value.type() == QVariant::String) {
                qIDec->setTheme(value.toString());
            }
            settings->endGroup();
        }

        // Set ILoader settings path
        Core::ILoader &loader = *Core::ILoader::instance();
        loader.setSettingsPath(QSettings::UserScope,
                               QString("%1/%2.settings.json").arg(userSettingsPath, qApp->applicationName()));
        loader.setSettingsPath(QSettings::SystemScope,
                               QString("%1/%2.settings.json").arg(systemSettingsPath, qApp->applicationName()));

        // Add initial routine to loader object pool
        auto initRoutine = new Core::InitialRoutine(screen);
        loader.addObject("choruskit_init_routine", initRoutine);
        QObject::connect(initRoutine, &Core::InitialRoutine::done, initRoutine, [&] {
            loader.removeObject(initRoutine); //
            delete initRoutine;
        });
    }

    void afterLoadPlugins() override {
        // Do nothing
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMAppExtension host;

    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);
    a.setOrganizationName("ChorusKit");
    a.setOrganizationDomain("ChorusKit");

    Core::ILoader loader;

#ifdef CONFIG_ENABLE_BREAKPAD
    QBreakpadInstance.setDumpPath(host.appDataDir() + "/crashes");
#endif

    return MyLoaderConfiguration().run();
}