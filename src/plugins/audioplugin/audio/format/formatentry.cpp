#include "formatentry.h"

namespace Audio {

    FormatEntry::FormatEntry(QObject *parent) : QObject(parent) {
    }

    FormatEntry::~FormatEntry() = default;

    QStringList FormatEntry::extensionHints() const {
        return {};
    }

}