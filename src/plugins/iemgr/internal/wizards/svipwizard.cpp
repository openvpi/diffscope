#include "svipwizard.h"

#include <QApplication>
#include <QFile>

#include <QFileInfo>
#include <QtMath>

namespace IEMgr ::Internal {

    SvipWizard::SvipWizard(QObject *parent) : IWizard(QStringLiteral("iemgr.SvipWizard"), parent) {
        setCategory("score.XiaoIce");
        setDisplayName([]() { return tr("SVIP file"); });
        setDescription([]() { return tr("XStudio 2.0 project file."); });
        setDisplayCategory(
            []() { return QApplication::translate("IEMgr::WizardCategory", "XiaoIce"); });
    }

    SvipWizard::~SvipWizard() {
    }

    QString SvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QStringLiteral("%1(%2);;").arg(tr("XStudio Svip Files"), "*.svip") +
               IWizard::filter(feature);
    }

    IWizard::Features SvipWizard::features() const {
        return ImportProject | ExportProject;
    }

    bool SvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizard::ImportProject: {
                break;
            }
            case IWizard::ExportProject: {
                break;
            }
            default:
                break;
        }
        return true;
    }

}