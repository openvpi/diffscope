#include "actionmanager.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QMCore/qmchronomap.h>
#include <QMWidgets/qmappextension.h>

#include <CoreApi/actiondomain.h>

namespace Core {

    static inline void makeDirectories(const QString &filePath) {
        QDir dir(QFileInfo(filePath).absoluteDir());
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
    }

    static QString actionLayoutsDataPath() {
        static QString path = qAppExt->appDataDir() + QStringLiteral("/actions/layouts.xml");
        return path;
    }

    static QString actionShortcutsDataPath() {
        static QString path = qAppExt->appDataDir() + QStringLiteral("/actions/shortcuts.json");
        return path;
    }

    static QString actionIconsDataPath() {
        static QString path = qAppExt->appDataDir() + QStringLiteral("/actions/icons.json");
        return path;
    }

    class ActionManagerPrivate {
        Q_DECLARE_PUBLIC(ActionManager)
    public:
        ActionManagerPrivate() {
        }

        void init() {
            Q_Q(ActionManager);
            domain = new ActionDomain(q);
        }

        ActionManager *q_ptr;

        ActionDomain *domain;

        QMChronoMap<QString, QJsonObject> shortcutFamilies;
        QString currentShortcutFamily;
    };

    static ActionManager *m_instance = nullptr;

    ActionManager::ActionManager(QObject *parent)
        : ActionManager(*new ActionManagerPrivate(), parent) {
    }
    ActionManager::~ActionManager() {
        m_instance = nullptr;
    }
    ActionDomain *ActionManager::domain() const {
        Q_D(const ActionManager);
        return d->domain;
    }
    bool ActionManager::loadLayouts() {
        Q_D(ActionManager);
        QFile file(actionLayoutsDataPath());
        if (!file.exists()) {
            qDebug() << "Core::ActionManager: action layouts data not found";
            return false;
        }
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Core::ActionManager: failed to read action layouts data";
            return false;
        }
        d->domain->restoreLayouts(file.readAll());
        return true;
    }
    bool ActionManager::saveLayouts() const {
        Q_D(const ActionManager);
        QFile file(actionLayoutsDataPath());
        makeDirectories(file.fileName());
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Core::ActionManager: failed to create action layouts data";
            return false;
        }
        file.write(d->domain->saveLayouts());
        return true;
    }
    bool ActionManager::loadIcons() {
        Q_D(ActionManager);
        QFile file(actionIconsDataPath());
        if (!file.exists()) {
            qDebug() << "Core::ActionManager: action icons data not found";
            return false;
        }
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Core::ActionManager: failed to read action icons data";
            return false;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning().noquote().nospace()
                << "Core::ActionManager: failed to parse read action icons data: "
                << err.errorString();
            return false;
        }

        if (auto obj = doc.object(); !obj.isEmpty()) {
            d->domain->restoreOverriddenIcons(doc.object());
        }
        return true;
    }
    bool ActionManager::saveIcons() const {
        Q_D(const ActionManager);
        QFile file(actionIconsDataPath());
        makeDirectories(file.fileName());
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Core::ActionManager: failed to create action icons data";
            return false;
        }
        file.write(QJsonDocument(d->domain->saveOverriddenIcons()).toJson());
        return true;
    }
    bool ActionManager::loadShortcuts() {
        Q_D(ActionManager);
        QFile file(actionShortcutsDataPath());
        if (!file.exists()) {
            qDebug() << "Core::ActionManager: action shortcuts data not found";
            return false;
        }
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Core::ActionManager: failed to read action shortcuts data";
            return false;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning().noquote().nospace()
                << "Core::ActionManager: failed to parse read action shortcuts data: "
                << err.errorString();
            return false;
        }

        auto object = doc.object();
        if (auto obj = object.value(QStringLiteral("shortcuts")).toObject(); !obj.isEmpty()) {
            d->domain->restoreOverriddenShortcuts(obj);
        }
        d->currentShortcutFamily = object.value(QStringLiteral("family")).toString();
        return false;
    }
    bool ActionManager::saveShortcuts() const {
        Q_D(const ActionManager);

        QFile file(actionShortcutsDataPath());
        makeDirectories(file.fileName());
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Core::ActionManager: failed to create action shortcuts data";
            return false;
        }

        QJsonObject object;
        object.insert(QStringLiteral("shortcuts"), d->domain->saveOverriddenShortcuts());
        object.insert(QStringLiteral("family"), d->currentShortcutFamily);
        file.write(QJsonDocument(object).toJson());
        return true;
    }
    QString ActionManager::currentShortcutFamily() const {
        Q_D(const ActionManager);
        return d->currentShortcutFamily;
    }
    void ActionManager::setCurrentShortcutFamily(const QString &id) {
        Q_D(ActionManager);
        d->currentShortcutFamily = id;
    }
    QJsonObject ActionManager::shortcutFamily(const QString &id) const {
        Q_D(const ActionManager);
        return d->shortcutFamilies.value(id);
    }
    QStringList ActionManager::shortcutFamilies() const {
        Q_D(const ActionManager);
        return d->shortcutFamilies.keys_qlist();
    }
    void ActionManager::addShortcutFamily(const QString &id, const QJsonObject &family) {
        Q_D(ActionManager);
        d->shortcutFamilies.append(id, family);
    }
    void ActionManager::removeShortcutFamily(const QString &id) {
        Q_D(ActionManager);
        d->shortcutFamilies.remove(id);
    }
    void ActionManager::clearShortcutFamilies() {
        Q_D(ActionManager);
        d->shortcutFamilies.clear();
    }
    ActionManager::ActionManager(ActionManagerPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }

}