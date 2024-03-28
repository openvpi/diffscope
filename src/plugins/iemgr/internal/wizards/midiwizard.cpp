#include "midiwizard.h"

#include <QApplication>
#include <QMessageBox>

#include <cmath>
#include <set>

#include <coreplugin/ICore.h>

namespace IEMgr ::Internal {

    MidiWizard::MidiWizard(QObject *parent) : IWizard(QStringLiteral("iemgr.MidiWizard"), parent) {
        setCategory("score.SimpleScore");
        setDisplayName([]() { return tr("Midi file"); });
        setDescription([]() { return tr("A communications protocol for media interfaces."); });
        setDisplayCategory(
            []() { return QApplication::translate("IEMgr::WizardCategory", "Simple Score"); });
    }

    MidiWizard::~MidiWizard() {
    }

    IWizard::Features MidiWizard::features() const {
        return ImportProject | ExportProject;
    }

    QString MidiWizard::filter(Feature feature) const {
        const char *suffixes;
        switch (feature) {
            case IWizard::ImportProject:
                suffixes = "*.mid *.midi";
                break;
            default:
                suffixes = "*.mid";
                break;
        }
        return QStringLiteral("%1(%2);;").arg(tr("Standard Midi Files"), suffixes) +
               IWizard::filter(feature);
    }

    bool MidiWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
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