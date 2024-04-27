#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include <QAbstractButton>

#include <CoreApi/iwindow.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class IHomeWindowRegistry;

    class IHomeWindowPrivate;

    class CORE_EXPORT IHomeWindow : public IWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IHomeWindow)
    public:
        static IHomeWindow *instance();

    public:
        QAbstractButton *addTab(QWidget *w);
        void removeTab(QWidget *btn);

        void addNavButton(QAbstractButton *button);
        void removeNavButton(QAbstractButton *button);

        void addFileButton(QAbstractButton *button);
        void removeFileButton(QAbstractButton *button);

    protected:
        QString correctWindowTitle(const QString &title) const override;
        QWidget *createWindow(QWidget *parent) const override;
        void nextLoadingState(State nextState) override;

    protected:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        friend class IExecutiveRegistry<IHomeWindow>;
    };

    class CORE_EXPORT IHomeWindowRegistry : public IExecutiveRegistry<IHomeWindow> {
    public:
        static IHomeWindowRegistry *instance();
    };

}

#endif // IHOMEWINDOW_H
