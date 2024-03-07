#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

namespace Core::Internal {

    class HomeWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit HomeWindow(QWidget *parent = nullptr);
        ~HomeWindow();
    };

}

#endif // HOMEWINDOW_H
