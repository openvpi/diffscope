#include "opensvipwizard.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

#include <QtMath>

namespace IEMgr ::Internal {

    OpenSvipWizard::OpenSvipWizard(QObject *parent)
        : IWizard(QStringLiteral("iemgr.OpenSvipWizard"), parent) {
        setCategory("score.OpenVPI");
        setDisplayName([]() { return tr("OpenSVIP model"); });
        setDescription([]() { return tr("OpenSVIP converter intermediate model."); });
        setDisplayCategory(
            []() { return QApplication::translate("IEMgr::WizardCategory", "OpenVPI"); });
    }

    OpenSvipWizard::~OpenSvipWizard() {
    }

    QString OpenSvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QStringLiteral("%1(%2);;").arg(tr("OpenSVIP Json Files"), "*.json") +
               IWizard::filter(feature);
    }

    IWizard::Features OpenSvipWizard::features() const {
        return ImportProject | ExportProject;
    }

    bool OpenSvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizard::ImportProject: {
                break;
            }
            case IWizard::ExportProject:
                break;
            default:
                break;
        }
        return false;
    }

}