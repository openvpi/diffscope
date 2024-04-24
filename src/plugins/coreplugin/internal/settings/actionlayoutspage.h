#ifndef ACTIONLAYOUTSPAGE_H
#define ACTIONLAYOUTSPAGE_H

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

    class ActionLayoutsPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit ActionLayoutsPage(QObject *parent = nullptr);
        ~ActionLayoutsPage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QWidget *m_widget;
    };

}

#endif // ACTIONLAYOUTSPAGE_H
