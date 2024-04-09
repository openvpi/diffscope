#include "projectaddon.h"

#include <QtWidgets/QApplication>

#include "ihomewindow.h"
#include "icore.h"

namespace Core::Internal {

    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    ProjectAddOn::~ProjectAddOn() {
    }

    void ProjectAddOn::initialize() {
    }

    void ProjectAddOn::extensionsInitialized() {
    }

    void ProjectAddOn::reloadStrings() {
    }

}