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

        inline CNavFrame *nav() const {
            return m_navFrame;
        }

        inline QLayout *bottomButtonslayout() const {
            return m_bottomButtonsLayout;
        }

        QLayout *recentTopLayout() const;

    protected:
        CNavFrame *m_navFrame;
        QAbstractButton *m_titleButton;
        QAbstractButton *m_settingsButton;
        QAbstractButton *m_aboutButton;

        QAbstractButton *m_recentWidgetButton;
        HomeRecentWidget *m_recentWidget;

        QBoxLayout *m_bottomButtonsLayout;
        QFrame *m_bottomButtonsFrame;

    private:
        void _q_newButtonClicked();
        void _q_openButtonClicked();
        void _q_settingsButtonClicked();
        void _q_aboutButtonClicked();

        void _q_openFileRequested(const QString &fileName);
    };

}

#endif // HOMEWINDOW_H
