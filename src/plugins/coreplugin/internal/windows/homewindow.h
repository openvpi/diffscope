#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>

#include <QMWidgets/cnavframe.h>

namespace Core::Internal {

    class HomeRecentWidget;

    class HomeWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit HomeWindow(QWidget *parent = nullptr);
        ~HomeWindow();

        void reloadStrings();

        inline QAbstractButton *addTab(QWidget *w) {
            return m_navFrame->addWidget(w);
        }

        inline void removeTab(QWidget *w) {
            m_navFrame->removeWidget(w);
        }

        inline void addNavButton(QAbstractButton *button) {
            m_bottomButtonsLayout->addWidget(button);
        }

        inline void removeNavButton(QAbstractButton *button) {
            m_bottomButtonsLayout->removeWidget(button);
        }

        void addFileButton(QAbstractButton *button);
        void removeFileButton(QAbstractButton *button);

    protected:
        CNavFrame *m_navFrame;
        QAbstractButton *m_titleButton;
        QAbstractButton *m_configureButton;
        QAbstractButton *m_helpButton;

        QAbstractButton *m_recentWidgetButton;
        HomeRecentWidget *m_recentWidget;

        QBoxLayout *m_bottomButtonsLayout;
        QFrame *m_bottomButtonsFrame;

    private:
        void _q_newButtonClicked();
        void _q_openButtonClicked();
        void _q_configureButtonClicked();
        void _q_aboutButtonClicked();

        void _q_openFileRequested(const QString &fileName);
    };

}

#endif // HOMEWINDOW_H
