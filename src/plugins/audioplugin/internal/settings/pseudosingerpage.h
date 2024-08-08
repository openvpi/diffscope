#ifndef AUDIO_PSEUDOSINGERPAGE_H
#define AUDIO_PSEUDOSINGERPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Audio::Internal {

    class PseudoSingerPageWidget;

    class PseudoSingerPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit PseudoSingerPage(QObject *parent = nullptr);
        ~PseudoSingerPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<PseudoSingerPageWidget> m_widget;
    };

} // Internal

#endif // AUDIO_PSEUDOSINGERPAGE_H
