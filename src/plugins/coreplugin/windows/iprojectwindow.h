#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include <CoreApi/iwindow.h>
#include <CoreApi/actiondomain.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class IProjectWindowPrivate;

    class CORE_EXPORT IProjectWindow : public IWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IProjectWindow)
    public:
        explicit IProjectWindow(QObject *parent = nullptr);
        ~IProjectWindow();

    public:
        QMenuBar *menuBar() const override;
        void setMenuBar(QMenuBar *menuBar) override;

        QWidget *centralWidget() const override;
        void setCentralWidget(QWidget *widget) override;

        QStatusBar *statusBar() const override;
        void setStatusBar(QStatusBar *statusBar) override;

    protected:
        QString correctWindowTitle(const QString &title) const override;
        QWidget *createWindow(QWidget *parent) const override;
        void nextLoadingState(State nextState) override;
    };

}

#endif // IPROJECTWINDOW_H
