#ifndef ENVIROMENTTOPPAGE_H
#define ENVIROMENTTOPPAGE_H

#include <CoreApi/isettingpage.h>

namespace Core::Internal {

    class EnvironmentTopPage : public ISettingPage {
        Q_OBJECT
    public:
        explicit EnvironmentTopPage(QObject *parent = nullptr);
        ~EnvironmentTopPage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        // QWidget *m_widget;
    };

}

#endif // ENVIROMENTTOPPAGE_H
