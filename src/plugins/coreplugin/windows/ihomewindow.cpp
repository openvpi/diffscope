#include "ihomewindow.h"

#include <CoreApi/private/iwindow_p.h>

#include "appextra.h"
#include "homewindow.h"

namespace Core {

    static IHomeWindow *m_instance = nullptr;

    class IHomeWindowPrivate : IWindowPrivate {
        Q_DECLARE_PUBLIC(IHomeWindow)
    public:
    };

    IHomeWindow::IHomeWindow(QObject *parent)
        : IWindow(*new IWindowPrivate(), QStringLiteral("home"), parent) {
        m_instance = this;
    }

    IHomeWindow::~IHomeWindow() {
        m_instance = nullptr;
    }

    IHomeWindow *IHomeWindow::instance() {
        return m_instance;
    }

    QAbstractButton *IHomeWindow::addTab(QWidget *w) {
        return static_cast<Internal::HomeWindow *>(window())->addTab(w);
    }

    void IHomeWindow::removeTab(QWidget *btn) {
        return static_cast<Internal::HomeWindow *>(window())->removeTab(btn);
    }

    void IHomeWindow::addBottomButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->addBottomButton(button);
    }

    void IHomeWindow::removeBottomButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->removeBottomButton(button);
    }

    void IHomeWindow::addRecentTopButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->recentTopLayout()->addWidget(button);
    }

    void IHomeWindow::removeRecentTopButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->recentTopLayout()->removeWidget(
            button);
    }

    QString IHomeWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(AppExtra::displayTitle(title));
    }

    QWidget *IHomeWindow::createWindow(QWidget *parent) const {
        return new Internal::HomeWindow();
    }

    void IHomeWindow::nextLoadingState(IWindow::State nextState) {
    }

}