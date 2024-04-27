#include "ihomewindow.h"

#include <CoreApi/private/iwindow_p.h>

#include "appextra.h"
#include "homewindow.h"

namespace Core {

    static IHomeWindow *m_instance = nullptr;

    IHomeWindow *IHomeWindow::instance() {
        return m_instance;
    }

    QAbstractButton *IHomeWindow::addTab(QWidget *w) {
        return static_cast<Internal::HomeWindow *>(window())->addTab(w);
    }

    void IHomeWindow::removeTab(QWidget *btn) {
        return static_cast<Internal::HomeWindow *>(window())->removeTab(btn);
    }

    void IHomeWindow::addNavButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->addNavButton(button);
    }

    void IHomeWindow::removeNavButton(QAbstractButton *button) {
        static_cast<Internal::HomeWindow *>(window())->removeNavButton(button);
    }

    void IHomeWindow::addFileButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->addFileButton(button);
    }

    void IHomeWindow::removeFileButton(QAbstractButton *button) {
        return static_cast<Internal::HomeWindow *>(window())->removeFileButton(button);
    }

    QString IHomeWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(AppExtra::displayTitle(title));
    }

    QWidget *IHomeWindow::createWindow(QWidget *parent) const {
        return new Internal::HomeWindow();
    }

    void IHomeWindow::nextLoadingState(State nextState) {
    }

    IHomeWindow::IHomeWindow(QObject *parent) : IWindow(*new IWindowPrivate(), parent) {
        m_instance = this;
    }

    IHomeWindow::~IHomeWindow() {
        m_instance = nullptr;
    }

    IHomeWindowRegistry *IHomeWindowRegistry::instance() {
        static IHomeWindowRegistry reg;
        return &reg;
    }

}