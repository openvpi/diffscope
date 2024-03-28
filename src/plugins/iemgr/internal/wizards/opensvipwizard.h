#ifndef CHORUSKIT_OPENSVIPWIZARD_H
#define CHORUSKIT_OPENSVIPWIZARD_H

#include <iemgr/iwizard.h>

namespace IEMgr::Internal {

    class OpenSvipWizard : public IWizard {
        Q_OBJECT
    public:
        explicit OpenSvipWizard(QObject *parent = nullptr);
        ~OpenSvipWizard();

    public:
        Features features() const override;
        QString filter(Feature feature) const override;
        bool runWizard(Feature feature, const QString &path, QWidget *parent) override;

    private:
    };

}

#endif // CHORUSKIT_OPENSVIPWIZARD_H
