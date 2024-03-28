#ifndef CHORUSKIT_MIDIWIZARD_H
#define CHORUSKIT_MIDIWIZARD_H

#include <iemgr/iwizard.h>

namespace IEMgr::Internal {

    class MidiWizard : public IWizard {
        Q_OBJECT
    public:
        explicit MidiWizard(QObject *parent = nullptr);
        ~MidiWizard();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;
    };

}

#endif // CHORUSKIT_MIDIWIZARD_H
