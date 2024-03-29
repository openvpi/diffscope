#include "homeaddon.h"

#include <QtWidgets/QApplication>

#include <QMWidgets/ctabbutton.h>
#include <QMWidgets/qmdecoratorv2.h>

#include <coreplugin/ihomewindow.h>

#include "wizardmanager.h"

namespace IEMgr::Internal {

    HomeAddOn::HomeAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    HomeAddOn::~HomeAddOn() = default;

    void HomeAddOn::initialize() {
        importButton = new CTabButton();
        connect(importButton, &QAbstractButton::clicked, this, &HomeAddOn::_q_importButtonClicked);
        windowHandle()->cast<Core::IHomeWindow>()->addFileButton(importButton);

        qIDec->installLocale(this, _LOC(HomeAddOn, this));
    }

    void HomeAddOn::extensionsInitialized() {
    }

    void HomeAddOn::reloadStrings() {
        importButton->setText(QApplication::translate("Application", "Import"));
    }

    void HomeAddOn::_q_importButtonClicked() {
        WizardManager::instance()->runImport(windowHandle()->window());
    }

}