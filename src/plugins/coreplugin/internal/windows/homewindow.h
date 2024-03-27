#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

#include <QMWidgets/cnavframe.h>

namespace Core::Internal {

    class HomeWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit HomeWindow(QWidget *parent = nullptr);
        ~HomeWindow();

        void reloadStrings();

    public:
        CNavFrame *navFrame;
        QAbstractButton *titleButton;
        QAbstractButton *aboutButton;

        QAbstractButton *recentWidgetButton;

    private:
        void _q_newButtonClicked();
        void _q_openButtonClicked();
        void _q_aboutButtonClicked();

        void _q_openFileRequested(const QString &fileName);
    };

}

#endif // HOMEWINDOW_H
