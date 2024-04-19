#ifndef AUDIO_OUTPUTPLAYBACKPAGE_H
#define AUDIO_OUTPUTPLAYBACKPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;
class QSlider;

namespace SVS {
    class ExpressionDoubleSpinBox;
};

namespace Audio {

    class OutputPlaybackPage : public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit OutputPlaybackPage(QObject *parent = nullptr);
        ~OutputPlaybackPage() override;

        QString sortKeyword() const override;

        QWidget *widget() override;

        bool accept() override;
        void finish() override;

    private:
        QPointer<QWidget> m_widget;
        QComboBox *m_driverComboBox;
        QComboBox *m_deviceComboBox;
        QComboBox *m_bufferSizeComboBox;
        QComboBox *m_sampleRateComboBox;
        QComboBox *m_hotPlugModeComboBox;
        QSlider *m_deviceGainSlider;
        SVS::ExpressionDoubleSpinBox *m_deviceGainSpinBox;
        QSlider *m_devicePanSlider;
        SVS::ExpressionDoubleSpinBox *m_devicePanSpinBox;
        QComboBox *m_playHeadBehaviorComboBox;
        QCheckBox *m_closeDeviceOnPlaybackStopCheckBox;
        SVS::ExpressionDoubleSpinBox *m_fileBufferSizeMsecSpinBox;

        void updateDriverComboBox();
        void updateDeviceComboBox();
        void updateBufferSizeAndSampleRateComboBox();

        void updateGain(double gain);
        void updatePan(double pan);
    };

} // Audio

#endif // AUDIO_OUTPUTPLAYBACKPAGE_H
