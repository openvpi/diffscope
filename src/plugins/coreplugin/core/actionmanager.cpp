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
        QPair<QString, bool> currentFamily;
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
        d->currentFamily.second = object.value(QStringLiteral("system")).toBool();
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
        object.insert(QStringLiteral("system"), d->currentFamily.second);
        file.write(QJsonDocument(object).toJson());
        return true;
    }
    QPair<QString, bool> ActionManager::currentShortcutsFamily() const {
        Q_D(const ActionManager);
        return d->currentFamily;
    }
    bool ActionManager::setCurrentShortcutsFamily(const QString &id, bool system) {
        Q_D(ActionManager);
        if (!(system ? d->systemFamilies : d->userFamilies).contains(id)) {
            return false;
        }
        d->currentFamily = {id, system};
        return true;
    }
    QStringList ActionManager::systemShortcutsFamilies() const {
        Q_D(const ActionManager);
        return d->systemFamilies.keys_qlist();
    }
    ActionDomain::ShortcutsFamily ActionManager::systemShortcutsFamily(const QString &id) const {
        Q_D(const ActionManager);
        return d->systemFamilies.value(id);
    }
    bool ActionManager::addSystemShortcutsFamily(const QString &id,
                                                 const ActionDomain::ShortcutsFamily &family) {
        Q_D(ActionManager);
        return d->systemFamilies.append(id, family, false).second;
    }
    bool ActionManager::removeSystemShortcutsFamily(const QString &id) {
        Q_D(ActionManager);
        return d->systemFamilies.remove(id);
    }
    void ActionManager::clearSystemShortcutsFamily() {
        Q_D(ActionManager);
        d->systemFamilies.clear();
    }
    QStringList ActionManager::userShortcutFamilies() const {
        Q_D(const ActionManager);
        return d->userFamilies.keys_qlist();
    }
    ActionDomain::ShortcutsFamily ActionManager::userShortcutsFamily(const QString &id) const {
        Q_D(const ActionManager);
        return d->userFamilies.value(id);
    }
    bool ActionManager::addUserShortcutsFamily(const QString &id,
                                               const ActionDomain::ShortcutsFamily &family) {
        Q_D(ActionManager);
        return d->userFamilies.append(id, family, false).second;
    }
    bool ActionManager::renameUserShortcutsFamily(const QString &id, const QString &newId) {
        Q_D(ActionManager);
        auto it = d->userFamilies.find(id);
        if (it == d->userFamilies.end()) {
            return false;
        }

        if (d->userFamilies.contains(newId)) {
            return false;
        }

        auto it2 = std::next(it);
        auto val = it.value();
        d->userFamilies.erase(it);
        d->userFamilies.insert(it2, newId, val);
        return true;
    }
    bool ActionManager::removeUserShortcutsFamily(const QString &id) {
        Q_D(ActionManager);
        return d->userFamilies.remove(id);
    }
    void ActionManager::clearUserShortcutsFamilies() {
        Q_D(ActionManager);
        return d->userFamilies.clear();
    }

}