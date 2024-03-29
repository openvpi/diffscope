#ifndef HOMEADDON_H
#define HOMEADDON_H

#include <QtWidgets/QAbstractButton>

#include <CoreApi/iwindowaddon.h>

namespace Core::Internal {

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
        QAbstractButton *newButton;
        QAbstractButton *openButton;

    private:
        void _q_newButtonClicked();
        void _q_openButtonClicked();
    };

}

#endif // HOMEADDON_H
