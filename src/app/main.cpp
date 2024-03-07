#include <QApplication>
#include <QDebug>
#include <QNetworkProxyFactory>

#include <extensionsystem/pluginmanager.h>

#ifdef CONFIG_ENABLE_BREAKPAD
#  include <QBreakpadHandler.h>
#endif

#include <QMCore/QMSystem.h>
#include <QMWidgets/QMAppExtension.h>
#include <QMWidgets/QMDecoratorV2.h>

#include <CoreApi/iloader.h>

#include <CkLoader/loaderconfig.h>

#include <choruskit_config.h>

class MyLoaderConfiguration : public Loader::LoaderConfiguration {
public:
    MyLoaderConfiguration() {
        allowRoot = false;
        pluginIID = QStringLiteral(APP_PLUGIN_IID);
        splashSettingPath = qAppExt->appShareDir() + QStringLiteral("/config.json");
        userSettingsPath = qAppExt->appDataDir();
        systemSettingsPath = qAppExt->appShareDir();

        pluginPaths << qAppExt->appPluginsDir();
    }

    bool preprocessArguments(QStringList &arguments, int *code) override {
        // Skip if show help
        const auto &realArgs = QApplication::arguments();
        const auto &realArgsSet = QSet<QString>(realArgs.begin(), realArgs.end());
        if (realArgsSet.contains(QStringLiteral("-h")) ||
            realArgsSet.contains(QStringLiteral("--help"))) {
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
        loader.setSettingsPath(
            QSettings::UserScope,
            QStringLiteral("%1/%2.settings.json").arg(userSettingsPath, QStringLiteral(APP_NAME)));
        loader.setSettingsPath(QSettings::SystemScope,
                               QStringLiteral("%1/%2.settings.json")
                                   .arg(systemSettingsPath, QStringLiteral(APP_NAME)));

        // Add initial routine to loader object pool
        auto initRoutine = new Core::InitialRoutine(screen);
        loader.addObject(QStringLiteral("choruskit_init_routine"), initRoutine);
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
    a.setApplicationName(QStringLiteral(APP_NAME));
    a.setApplicationVersion(QStringLiteral(APP_VERSION));
    a.setOrganizationName(QStringLiteral(APP_ORG_NAME));
    a.setOrganizationDomain(QStringLiteral(APP_ORG_DOMAIN));

    QMAppExtension host;

#ifdef CONFIG_ENABLE_BREAKPAD
    QBreakpadInstance.setDumpPath(host.appDataDir() + "/crashes");
#endif

    Core::ILoader loader;
    return MyLoaderConfiguration().run();
}