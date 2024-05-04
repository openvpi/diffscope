#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtNetwork/QNetworkProxyFactory>
#include <QtWidgets/QApplication>
#include <QtWidgets/QToolTip>

#include <extensionsystem/pluginmanager.h>

#ifdef CONFIG_ENABLE_BREAKPAD
#  include <QBreakpadHandler.h>
#endif

#include <QMCore/qmsystem.h>
#include <QMWidgets/qmappextension.h>
#include <QMWidgets/qmdecoratorv2.h>

#include <CoreApi/iloader.h>
#include <CoreApi/private/iloader_p.h>

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

    void splashWillShow(QSplashScreen *screen) override {
        splash = screen;

        // Don't show plugin manager debug info
        QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));
    }

    void beforeLoadPlugins() override {
        // Create app data path and temp path
        if (!qAppExt->createAppDirs()) {
            showError(QCoreApplication::translate("Application",
                                                  "Failed to create data or temp directories!"));
        }

        // Set global settings path
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, userSettingsPath);
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, systemSettingsPath);

        // Make sure we honor the system's proxy settings
        QNetworkProxyFactory::setUseSystemConfiguration(true);

        // Restore language and themes
        {
            auto settings = ExtensionSystem::PluginManager::settings();
            settings->beginGroup(QStringLiteral("Preferences"));
            auto value = settings->value(QStringLiteral("Translation"));
            if (!value.isNull()) {
                qIDec->setLocale(value.toString());
            }

            value = settings->value(QStringLiteral("Theme"));
            if (!value.isNull()) {
                qIDec->setTheme(value.toString());
            }

            value = settings->value(QStringLiteral("Zoom"));
            if (!value.isNull()) {
                qIDec->setZoomRatio(value.toString().toDouble() / 100);
            }

            value = settings->value(QStringLiteral("Font"));
            {
                // Default font
                QFont font(QMAppExtension::systemDefaultFont());
                font.setPixelSize(12);

                // Get font
                if (!value.isNull()) {
                    font.fromString(value.toString());
                }

                // Normalize font
                if (font.pixelSize() <= 0) {
                    font.setPixelSize(12);
                }
                qIDec->setFontRatio(font.pixelSize() / 12.0);

                // The application font is determined at startup and remains unchanged, using the
                // `userFont` property to record the changes made by the user during the current
                // application life.
                qApp->setProperty("userFont", font);
                qApp->setProperty("userFontInitial", font);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                font.setPointSize(font.pixelSize() * 0.75 * qIDec->zoomRatio());
#else
                font.setPixelSize(font.pixelSize() * qIDec->zoomRatio());
#endif
                // Make the default font update with system dpi
                QGuiApplication::setFont(font);
                QToolTip::setFont(font);
            }

            value = settings->value(QStringLiteral("UseSystemFont"));
            {
                // Default value
                qApp->setProperty("useSystemFont", true);
                // Get value
                if (value.type() == QVariant::String) {
                    qApp->setProperty("useSystemFont",
                                      value.toString().toLower() == QStringLiteral("true"));
                }
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

        // Pass splash screen to core plugin
        Core::ILoaderPrivate::quickData(0) = splash;
    }

    void afterLoadPlugins() override {
        // Do nothing
    }

    QSplashScreen *splash = nullptr;
};

int main(int argc, char *argv[]) {
    // Make sure Qt uses the plugin path in qt.conf
    if (qEnvironmentVariableIsSet("QT_PLUGIN_PATH")) {
        qputenv("QT_PLUGIN_PATH", {});
    }

    // Frameless window requires it
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QApplication a(argc, argv);
    a.setApplicationName(QStringLiteral(APP_NAME));
    a.setApplicationVersion(QStringLiteral(APP_VERSION));
    a.setOrganizationName(QStringLiteral(APP_ORG_NAME));
    a.setOrganizationDomain(QStringLiteral(APP_ORG_DOMAIN));

    QMAppExtension host;

#ifdef CONFIG_ENABLE_BREAKPAD
    QBreakpadInstance.setDumpPath(host.appDataDir() + QStringLiteral("/crashes"));
#endif

    Core::ILoader loader;
    return MyLoaderConfiguration().run();
}