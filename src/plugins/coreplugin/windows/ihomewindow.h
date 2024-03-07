#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include <CoreApi/iwindow.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class IHomeWindowPrivate;

    class IHomeWindow : public IWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IHomeWindow)
    public:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        static IHomeWindow *instance();

    protected:
        QString correctWindowTitle(const QString &title) const override;
        QWidget *createWindow(QWidget *parent) const override;
        void nextLoadingState(Core::IWindow::State nextState) override;
    };

}

#endif // IHOMEWINDOW_H
