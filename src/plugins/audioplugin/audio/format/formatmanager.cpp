#include "formatmanager.h"
#include "formatmanager_p.h"

#include <audioplugin/formatentry.h>

namespace Audio {
    FormatManager::FormatManager(QObject *parent) : QObject(parent), d_ptr(new FormatManagerPrivate) {
        Q_D(FormatManager);
        d->q_ptr = this;
    }

    FormatManager::~FormatManager() = default;

    void FormatManager::addEntry(FormatEntry *entry) {
        Q_D(FormatManager);
        d->entries.append(entry);
    }

    QList<FormatEntry *> FormatManager::entries() const {
        Q_D(const FormatManager);
        return d->entries;
    }

    QStringList FormatManager::filters() const {
        Q_D(const FormatManager);
        QStringList list;
        for (auto entry : d->entries)
            list += entry->filters();
        return list;
    }

    FormatEntry *FormatManager::getEntryByFilter(const QString &filter) const {
        Q_D(const FormatManager);
        for (auto entry : d->entries) {
            if (entry->filters().contains(filter))
                return entry;
        }
        return nullptr;
    }

}