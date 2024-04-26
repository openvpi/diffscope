#ifndef PROJECTADDON_H
#define PROJECTADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindow.h>
#include <CoreApi/actionitem.h>

namespace IEMgr::Internal {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();

    protected:
        void initActions();

        struct ActionItems {
            Core::ActionItem *ImportProject;
            Core::ActionItem *AppendProject;
            Core::ActionItem *ExportProject;
        };

        ActionItems ai;
    };

}

#endif // PROJECTADDON_H
