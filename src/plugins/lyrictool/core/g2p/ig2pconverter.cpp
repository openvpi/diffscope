#include "ig2pconverter.h"
#include "ig2pconverter_p.h"

namespace LyricTool {

    IG2pConverterPrivate::IG2pConverterPrivate() {
    }

    IG2pConverterPrivate::~IG2pConverterPrivate() {
    }

    void IG2pConverterPrivate::init() {
    }

    IG2pConverter::IG2pConverter(const QString &id, QObject *parent)
        : IG2pConverter(*new IG2pConverterPrivate(), id, parent) {
    }

    IG2pConverter::~IG2pConverter() = default;

    QList<LyricInfo> IG2pConverter::convert(const QStringList &input,
                                            const QVariantMap &args) const {
        return {};
    }

    QString IG2pConverter::id() const {
        Q_D(const IG2pConverter);
        return d->id;
    }

    QString IG2pConverter::displayName() const {
        Q_D(const IG2pConverter);
        return d->displayName;
    }

    void IG2pConverter::setDisplayName(const QMDisplayString &displayName) {
        Q_D(IG2pConverter);
        d->displayName = displayName;
    }

    QString IG2pConverter::description() const {
        Q_D(const IG2pConverter);
        return d->description;
    }

    void IG2pConverter::setDescription(const QMDisplayString &description) {
        Q_D(IG2pConverter);
        d->description = description;
    }

    IG2pConverter::IG2pConverter(IG2pConverterPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
