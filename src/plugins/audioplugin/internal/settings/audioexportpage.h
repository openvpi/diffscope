#ifndef AUDIO_AUDIOEXPORTPAGE_H
#define AUDIO_AUDIOEXPORTPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

namespace Audio::Internal {

    class AudioExportPageWidget;
    
    class AudioExportPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit AudioExportPage(QObject *parent = nullptr);
        ~AudioExportPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<AudioExportPageWidget> m_widget;
    };

}

#endif // AUDIO_AUDIOEXPORTPAGE_H
