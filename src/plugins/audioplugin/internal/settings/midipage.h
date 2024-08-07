#ifndef AUDIO_MIDIPAGE_H
#define AUDIO_MIDIPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;

namespace Audio::Internal {

    class MIDIPageWidget;

    class MIDIPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit MIDIPage(QObject *parent = nullptr);
        ~MIDIPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<MIDIPageWidget> m_widget;
        QComboBox *m_midiDeviceComboBox = nullptr;

    };

}

#endif // AUDIO_MIDIPAGE_H
