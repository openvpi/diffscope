#ifndef USTWIZARD_H
#define USTWIZARD_H

#include <iemgr/iwizard.h>

namespace IEMgr::Internal {

    class UstWizard : public IWizard {
        Q_OBJECT
    public:
        explicit UstWizard(QObject *parent = nullptr);
        ~UstWizard();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;
    };

}

#endif // USTWIZARD_H
