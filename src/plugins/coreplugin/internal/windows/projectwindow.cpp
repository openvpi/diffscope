#include "projectwindow.h"

#include <QMenuBar>
#include <QVBoxLayout>

#include <QMWidgets/ctoolbar.h>
#include <QMWidgets/cmenubar.h>

#include <JetBrainsDockingSystem/dockwidget.h>

namespace Core::Internal {

    ProjectWindow::ProjectWindow(QWidget *parent) : QMainWindow(parent) {
        m_toolBar = new CToolBar();
        m_toolBar->setObjectName("main-toolbar");

        m_menuBar = new CMenuBar();
        m_menuBar->setObjectName("main-menubar");
        setMenuBar(m_menuBar);

        m_dock = new JBDS::DockWidget();
        m_dock->setObjectName("main-dock");

        auto widget = new QWidget();
        auto layout = new QVBoxLayout();
        layout->setContentsMargins({});
        layout->setSpacing(0);

        layout->addWidget(m_toolBar);
        layout->addWidget(m_dock);
        
        widget->setLayout(layout);
        setCentralWidget(widget);
    }

    ProjectWindow::~ProjectWindow() {
    }

    void ProjectWindow::reloadStrings() {
    }

}