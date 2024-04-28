#include "actionmanager.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

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

    static QJsonObject serializeShortcuts(const ActionDomain::ShortcutsFamily &shortcutsFamily) {
        QJsonObject rootObj;
        for (auto it = shortcutsFamily.begin(); it != shortcutsFamily.end(); ++it) {
            const auto &key = it.key();
            QJsonValue value = QJsonValue::Null;
            if (const auto &val = it.value()) {
                QJsonArray arr;
                for (const auto &subItem : val.value()) {
                    arr.push_back(subItem.toString());
                }
                value = arr;
            }
            rootObj.insert(key, value);
        }
        return rootObj;
    }

    static bool deserializeShortcuts(const QJsonObject &object,
                                     ActionDomain::ShortcutsFamily &out) {
        ActionDomain::ShortcutsFamily result;
        for (auto it = object.begin(); it != object.end(); ++it) {
            const auto &key = it.key();
            const auto &value = it.value();
            if (value.isNull()) {
                result.insert(key, {});
                continue;
            }
            auto arr = value.toObject();
            QList<QKeySequence> shortcuts;
            shortcuts.reserve(arr.size());
            for (const auto &item : std::as_const(arr)) {
                QKeySequence ks = QKeySequence::fromString(item.toString());
                if (!ks.isEmpty()) {
                    shortcuts.append(ks);
                }
            }
            result.insert(key, shortcuts);
        }
        out = result;
        return true;
    }

    static QJsonObject serializeIcons(const ActionDomain::IconFamily &iconFamily) {
        QJsonObject rootObj;
        for (auto it = iconFamily.begin(); it != iconFamily.end(); ++it) {
            const auto &key = it.key();
            QJsonValue value = QJsonValue::Null;
            if (const auto &val = it.value()) {
                QJsonObject obj;
                obj.insert(QStringLiteral("fromFile"), val->fromFile());
                obj.insert(QStringLiteral("data"), val->data());
                value = obj;
            }
            rootObj.insert(key, value);
        }
        return rootObj;
    }

    static bool deserializeIcons(const QJsonObject &object, ActionDomain::IconFamily &out) {
        ActionDomain::IconFamily result;
        for (auto it = object.begin(); it != object.end(); ++it) {
            const auto &key = it.key();
            const auto &value = it.value();
            if (value.isNull()) {
                result.insert(key, {});
                continue;
            }
            auto obj = value.toObject();
            result.insert(key, ActionDomain::IconReference{
                                   obj.value(QStringLiteral("data")).toString(),
                                   obj.value(QStringLiteral("fromFile")).toBool(),
                               });
        }
        out = result;
        return true;
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

        QMChronoMap<QString, ActionDomain::ShortcutsFamily> systemFamilies;
        QMChronoMap<QString, ActionDomain::ShortcutsFamily> userFamilies;
        QPair<QString, ActionManager::Scope> currentFamily;
    };

    static ActionManager *m_instance = nullptr;

    ActionManager::ActionManager(QObject *parent)
        : QObject(parent), d_ptr(new ActionManagerPrivate()) {
        Q_D(ActionManager);
        m_instance = this;
        d->q_ptr = this;
        d->init();
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
            ActionDomain::IconFamily iconFamily;
            if (deserializeIcons(doc.object(), iconFamily)) {
                d->domain->setIconFamily(iconFamily);
            }
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
        file.write(QJsonDocument(serializeIcons(d->domain->iconFamily())).toJson());
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
            ActionDomain::ShortcutsFamily shortcutsFamily;
            if (deserializeShortcuts(doc.object(), shortcutsFamily)) {
                d->domain->setShortcutsFamily(shortcutsFamily);
            }
        }
        d->currentFamily.first = object.value(QStringLiteral("family")).toString();
        d->currentFamily.second = object.value(QStringLiteral("system")).toBool() ? System : User;
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
        object.insert(QStringLiteral("shortcuts"),
                      serializeShortcuts(d->domain->shortcutsFamily()));
        object.insert(QStringLiteral("family"), d->currentFamily.first);
        object.insert(QStringLiteral("system"), d->currentFamily.second == System);
        file.write(QJsonDocument(object).toJson());
        return true;
    }
    QPair<QString, ActionManager::Scope> ActionManager::currentShortcutsFamily() const {
        Q_D(const ActionManager);
        return d->currentFamily;
    }
    void ActionManager::setCurrentShortcutsFamily(const QString &id, ActionManager::Scope scope) {
        Q_D(ActionManager);
        d->currentFamily = {id, scope};
    }
    ActionDomain::ShortcutsFamily ActionManager::shortcutsFamily(const QString &id,
                                                                 ActionManager::Scope scope) const {
        Q_D(const ActionManager);
        auto &families = scope == System ? d->systemFamilies : d->userFamilies;
        return families.value(id);
    }
    QStringList ActionManager::shortcutFamilies(ActionManager::Scope scope) const {
        Q_D(const ActionManager);
        auto &families = scope == System ? d->systemFamilies : d->userFamilies;
        return families.keys_qlist();
    }
    void ActionManager::addShortcutsFamily(const QString &id,
                                          const ActionDomain::ShortcutsFamily &family,
                                          ActionManager::Scope scope) {
        Q_D(ActionManager);
        auto &families = scope == System ? d->systemFamilies : d->userFamilies;
        families.append(id, family);
    }
    void ActionManager::removeShortcutsFamily(const QString &id, ActionManager::Scope scope) {
        Q_D(ActionManager);
        auto &families = scope == System ? d->systemFamilies : d->userFamilies;
        families.remove(id);
    }
    void ActionManager::clearShortcutsFamilies(ActionManager::Scope scope) {
        Q_D(ActionManager);
        auto &families = scope == System ? d->systemFamilies : d->userFamilies;
        families.clear();
    }

}