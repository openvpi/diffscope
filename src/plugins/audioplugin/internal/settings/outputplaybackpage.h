#ifndef AUDIO_OUTPUTPLAYBACKPAGE_H
#define AUDIO_OUTPUTPLAYBACKPAGE_H

#include <QPointer>

#include <CoreApi/isettingpage.h>

class QComboBox;
class QCheckBox;

namespace SVS {
    class ExpressionDoubleSpinBox;
    class ExpressionSpinBox;
    class SeekBar;
};

namespace Audio::Internal {

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
        QComboBox *m_driverComboBox = nullptr;
        QComboBox *m_deviceComboBox = nullptr;
        QComboBox *m_bufferSizeComboBox = nullptr;
        QComboBox *m_sampleRateComboBox = nullptr;
        QComboBox *m_hotPlugModeComboBox = nullptr;
        SVS::SeekBar *m_deviceGainSlider = nullptr;
        SVS::ExpressionDoubleSpinBox *m_deviceGainSpinBox = nullptr;
        SVS::SeekBar *m_devicePanSlider = nullptr;
        SVS::ExpressionSpinBox *m_devicePanSpinBox = nullptr;
        QComboBox *m_playHeadBehaviorComboBox = nullptr;
        QCheckBox *m_closeDeviceOnPlaybackStopCheckBox = nullptr;
        SVS::ExpressionSpinBox *m_fileBufferingSizeSpinBox = nullptr;

        void updateDriverComboBox();
        void updateDeviceComboBox();
        void updateBufferSizeAndSampleRateComboBox();

        void updateGain(double gain);
        void updatePan(double pan);
    };

}

#endif // AUDIO_OUTPUTPLAYBACKPAGE_H
