#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include <QAbstractButton>

#include <CoreApi/iwindow.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class IHomeWindowPrivate;

    class CORE_EXPORT IHomeWindow : public IWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IHomeWindow)
    public:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        static IHomeWindow *instance();

    public:
        QAbstractButton *addTab(QWidget *w);
        void removeTab(QWidget *btn);

        void addBottomButton(QAbstractButton *button);
        void removeBottomButton(QAbstractButton *button);

        void addRecentTopButton(QAbstractButton *button);
        void removeRecentTopButton(QAbstractButton *button);

    protected:
        QString correctWindowTitle(const QString &title) const override;
        QWidget *createWindow(QWidget *parent) const override;
        void nextLoadingState(Core::IWindow::State nextState) override;
    };

}

#endif // IHOMEWINDOW_H
