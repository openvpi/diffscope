#ifndef PROJECTADDON_H
#define PROJECTADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindowaddon.h>

namespace Core::Internal {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();
    };

}

#endif // PROJECTADDON_H
