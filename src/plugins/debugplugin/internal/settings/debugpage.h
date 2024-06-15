#ifndef DEBUG_DEBUGPAGE_H
#define DEBUG_DEBUGPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Debug::Internal {

    class DebugPageWidget;
    
    class DebugPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit DebugPage(QObject *parent = nullptr);
        ~DebugPage();

    public:
        QString sortKeyword() const override;

        bool matches(const QString &word) const override;
        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<DebugPageWidget> m_widget;
    };

}

#endif // DEBUG_DEBUGPAGE_H
