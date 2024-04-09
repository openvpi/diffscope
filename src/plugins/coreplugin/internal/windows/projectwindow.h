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
    };

}

#endif // PROJECTWINDOW_H
