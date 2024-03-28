#ifndef CHORUSKIT_SVIPWIZARD_H
#define CHORUSKIT_SVIPWIZARD_H

#include <iemgr/iwizard.h>

namespace IEMgr::Internal {

    class SvipWizard : public IWizard {
        Q_OBJECT
    public:
        explicit SvipWizard(QObject *parent = nullptr);
        ~SvipWizard();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;
    };

}

#endif // CHORUSKIT_SVIPWIZARD_H
