#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QMainWindow>

namespace Core::Internal {

    class ProjectWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit ProjectWindow(QWidget *parent = nullptr);
        ~ProjectWindow();

        void reloadStrings();

        inline QToolBar *toolBar() const {
            return m_toolBar;
        }

    protected:
        QToolBar *m_toolBar;
        QMenuBar *m_menuBar;
        QWidget *m_dock;
    };

}

#endif // PROJECTWINDOW_H
