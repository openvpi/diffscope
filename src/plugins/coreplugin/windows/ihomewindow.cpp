#include "ihomewindow.h"

#include <CoreApi/private/iwindow_p.h>

#include "icore.h"
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
        return static_cast<Internal::HomeWindow *>(window())->nav()->addWidget(w);
    }

    void IHomeWindow::removeTab(QAbstractButton *btn) {
        return static_cast<Internal::HomeWindow *>(window())->nav()->removeWidget(btn);
    }

    void IHomeWindow::addBottomButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->bottomButtonslayout()->addWidget(button);
    }

    void IHomeWindow::removeBottomButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->bottomButtonslayout()->removeWidget(button);
    }

    void IHomeWindow::addRecentTopButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->recentTopLayout()->addWidget(button);
    }

    void IHomeWindow::removeRecentTopButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->recentTopLayout()->removeWidget(
            button);
    }

    QString IHomeWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(ICore::displayTitle(title));
    }

    QWidget *IHomeWindow::createWindow(QWidget *parent) const {
        return new Internal::HomeWindow();
    }

    void IHomeWindow::nextLoadingState(IWindow::State nextState) {
    }

}