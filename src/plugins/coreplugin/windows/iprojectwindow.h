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
        enum Mode {
            Standalone,
            VST,
        };

        Mode mode() const;
        inline bool isVST() const;

    public:
        QMenuBar *menuBar() const override;
        void setMenuBar(QMenuBar *menuBar) override;

        QWidget *centralWidget() const override;
        void setCentralWidget(QWidget *widget) override;

        QStatusBar *statusBar() const override;
        void setStatusBar(QStatusBar *statusBar) override;

        QToolBar *mainToolbar() const;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    protected:
        QString correctWindowTitle(const QString &title) const override;
        QWidget *createWindow(QWidget *parent) const override;
        void nextLoadingState(State nextState) override;

    protected:
        explicit IProjectWindow(QObject *parent = nullptr);
        explicit IProjectWindow(Mode mode, QObject *parent = nullptr);
        ~IProjectWindow();

        friend class IExecutiveRegistry<IProjectWindow>;
    };

    inline bool IProjectWindow::isVST() const {
        return mode() == VST;
    }

    class CORE_EXPORT IProjectWindowRegistry : public IExecutiveRegistry<IProjectWindow> {
    public:
        static IProjectWindowRegistry *instance();
    };

}

#endif // IPROJECTWINDOW_H
