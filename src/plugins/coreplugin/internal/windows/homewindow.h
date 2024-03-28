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

        inline void removeTab(QWidget *btn) {
            m_navFrame->removeWidget(btn);
        }

        inline void addBottomButton(QAbstractButton *button) {
            m_bottomButtonsLayout->addWidget(button);
        }

        inline void removeBottomButton(QAbstractButton *button) {
            m_bottomButtonsLayout->removeWidget(button);
        }

        QLayout *recentTopLayout() const;

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
