#ifndef IWIZARDPRIVATE_H
#define IWIZARDPRIVATE_H

#include <iemgr/iwizard.h>

namespace IEMgr {

    class IWizardPrivate {
        Q_DECLARE_PUBLIC(IWizard)
    public:
        IWizardPrivate();
        virtual ~IWizardPrivate();

        void init();

        IWizard *q_ptr;

        QString id;

        QMDisplayString displayName;
        QMDisplayString description;

        QString iconPath;

        QString category;
        QMDisplayString displayCategory;
    };

}

#endif // IWIZARDPRIVATE_H