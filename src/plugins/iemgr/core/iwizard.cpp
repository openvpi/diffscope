#include "iwizard.h"
#include "iwizard_p.h"

#include <QMCore/QMSystem.h>

#include <coreplugin/appextra.h>

namespace IEMgr {

    IWizardPrivate::IWizardPrivate() {
    }

    IWizardPrivate::~IWizardPrivate() {
    }

    void IWizardPrivate::init() {
    }

    IWizard::IWizard(const QString &id, QObject *parent)
        : IWizard(*new IWizardPrivate(), id, parent) {
    }

    IWizard::~IWizard() {
    }

    IWizard::IWizard(IWizardPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

    QString IWizard::id() const {
        Q_D(const IWizard);
        return d->id;
    }

    QString IWizard::filter(Feature feature) const {
        Q_UNUSED(feature);
        return QStringLiteral("%1(%2)").arg(tr("All Files"), Core::AppExtra::allFilesFilter());
    }

    QString IWizard::iconPath() const {
        Q_D(const IWizard);
        return d->iconPath;
    }

    void IWizard::setIconPath(const QString &path) {
        Q_D(IWizard);
        d->iconPath = path;
    }

    QMDisplayString IWizard::description() const {
        Q_D(const IWizard);
        return d->description;
    }

    void IWizard::setDescription(const QMDisplayString &description) {
        Q_D(IWizard);
        d->description = description;
    }

    QMDisplayString IWizard::displayName() const {
        Q_D(const IWizard);
        return d->displayName;
    }

    void IWizard::setDisplayName(const QMDisplayString &displayName) {
        Q_D(IWizard);
        d->displayName = displayName;
    }

    QString IWizard::category() const {
        Q_D(const IWizard);
        return d->category;
    }

    void IWizard::setCategory(const QString &category) {
        Q_D(IWizard);
        d->category = category;
    }

    QMDisplayString IWizard::displayCategory() const {
        Q_D(const IWizard);
        return d->displayCategory;
    }

    void IWizard::setDisplayCategory(const QMDisplayString &displayCategory) {
        Q_D(IWizard);
        d->displayCategory = displayCategory;
    }

}
