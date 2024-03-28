#ifndef HOMEADDON_H
#define HOMEADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindowaddon.h>

namespace IEMgr::Internal {

    class HomeAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit HomeAddOn(QObject *parent = nullptr);
        ~HomeAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();

    protected:
        QAbstractButton *importButton;

    private:
        void _q_importButtonClicked();
    };

}

#endif // HOMEADDON_H
