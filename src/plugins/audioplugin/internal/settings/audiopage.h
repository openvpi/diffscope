#ifndef AUDIO_AUDIOPAGE_H
#define AUDIO_AUDIOPAGE_H

#include <CoreApi/isettingpage.h>

namespace Audio::Internal {

    class AudioPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit AudioPage(QObject *parent = nullptr);
        ~AudioPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;
    };

}

#endif // AUDIO_AUDIOPAGE_H
