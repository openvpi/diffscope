#include "homeaddon.h"

#include <QtWidgets/QApplication>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmdecoratorv2.h>

#include "ihomewindow.h"
#include "icore.h"

namespace Core::Internal {

    HomeAddOn::HomeAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    HomeAddOn::~HomeAddOn() = default;

    void HomeAddOn::initialize() {
        newButton = new CTabButton();
        connect(newButton, &QAbstractButton::clicked, this, &HomeAddOn::_q_newButtonClicked);

        openButton = new CTabButton();
        connect(openButton, &QAbstractButton::clicked, this, &HomeAddOn::_q_openButtonClicked);

        auto iWin = windowHandle()->cast<Core::IHomeWindow>();
        iWin->addFileButton(newButton);
        iWin->addFileButton(openButton);

        qIDec->installLocale(this, _LOC(HomeAddOn, this));
    }

    void HomeAddOn::extensionsInitialized() {
    }

    void HomeAddOn::reloadStrings() {
        newButton->setText(QApplication::translate("Application", "New"));
        openButton->setText(QApplication::translate("Application", "Open"));
    }

    void HomeAddOn::_q_newButtonClicked() {
        Q_UNUSED(this)
        ICore::instance()->newFile();
    }

    void HomeAddOn::_q_openButtonClicked() {
        ICore::instance()->openFile({}, windowHandle()->window());
    }

}