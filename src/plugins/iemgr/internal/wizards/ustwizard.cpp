#include "ustwizard.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <coreplugin/icore.h>

namespace IEMgr ::Internal {

    UstWizard::UstWizard(QObject *parent) : IWizard(QStringLiteral("iemgr.UstWizard"), parent) {
        setCategory("score.SimpleScore");
        setDisplayName([]() { return tr("UST file"); });
        setDescription([]() { return tr("Utau sequence text file."); });
        setDisplayCategory(
            []() { return QApplication::translate("IEMgr::WizardCategory", "Simple Score"); });
    }

    UstWizard::~UstWizard() {
    }

    QString UstWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QStringLiteral("%1(%2);;").arg(tr("Utau Sequence Texts"), "*.ust") +
               IWizard::filter(feature);
    }

    IWizard::Features UstWizard::features() const {
        return ImportProject | ExportProject;
    }

    bool UstWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
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