#ifndef AUDIO_MIDIPAGE_H
#define AUDIO_MIDIPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;

namespace Audio {

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
        QPointer<QWidget> m_widget;
        QComboBox *m_midiDeviceComboBox;

    };

}

#endif // AUDIO_MIDIPAGE_H
