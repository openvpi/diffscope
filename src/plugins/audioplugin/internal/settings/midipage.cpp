#include "midipage.h"

#include <cmath>

#include <QDebug>
#include <QBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>

#include <TalcsMidi/MidiInputDevice.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/midisystem.h>

namespace Audio::Internal {

    inline int decibelToSliderValue(double decibel) {
        return static_cast<int>(std::pow(65536.0, (decibel + 96.0) / 96.0) - 1.0);
    }

    inline double sliderValueToDecibel(int value) {
        return 96.0 / std::log(65536) * std::log(value + 1.0) - 96.0;
    }

    class MIDIPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MIDIPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;

            auto inputGroupBox = new QGroupBox(tr("MIDI Input"));
            auto inputLayout = new QFormLayout;
            auto deviceComboBox = new QComboBox;
            inputLayout->addRow("Device", deviceComboBox);
            inputGroupBox->setLayout(inputLayout);
            mainLayout->addWidget(inputGroupBox);

            auto synthesizerGroupBox = new QGroupBox(tr("Synthesizer"));
            auto synthesizerLayout = new QFormLayout;
            auto generatorComboBox = new QComboBox;
            generatorComboBox->addItems({tr("Sine wave"), tr("Square wave"), tr("Triangle Wave"), tr("Sawtooth wave")});
            synthesizerLayout->addRow("Generator", generatorComboBox);

            auto amplitudeLayout = new QHBoxLayout;
            auto amplitudeSlider = new QSlider;
            amplitudeSlider->setOrientation(Qt::Horizontal);
            amplitudeSlider->setRange(0, 65535);
            amplitudeLayout->addWidget(amplitudeSlider);
            auto amplitudeSpinBox = new SVS::ExpressionDoubleSpinBox;
            amplitudeSpinBox->setRange(-96, 0);
            amplitudeSpinBox->setSpecialValueText("-INF");
            amplitudeLayout->addWidget(amplitudeSpinBox);
            auto amplitudeLabel = new QLabel(tr("Amplitude (dB)"));
            amplitudeLabel->setBuddy(amplitudeSpinBox);
            synthesizerLayout->addRow(amplitudeLabel, amplitudeLayout);

            auto attackLayout = new QHBoxLayout;
            auto attackSlider = new QSlider;
            attackSlider->setOrientation(Qt::Horizontal);
            attackSlider->setRange(0, 1000);
            attackLayout->addWidget(attackSlider);
            auto attackSpinBox = new SVS::ExpressionSpinBox;
            attackSpinBox->setRange(0, 1000);
            attackLayout->addWidget(attackSpinBox);
            auto attackLabel = new QLabel(tr("Attack (ms)"));
            attackLabel->setBuddy(attackSpinBox);
            synthesizerLayout->addRow(attackLabel, attackLayout);

            auto releaseLayout = new QHBoxLayout;
            auto releaseSlider = new QSlider;
            releaseSlider->setOrientation(Qt::Horizontal);
            releaseSlider->setRange(0, 1000);
            releaseLayout->addWidget(releaseSlider);
            auto releaseSpinBox = new SVS::ExpressionSpinBox;
            releaseSpinBox->setRange(0, 1000);
            releaseLayout->addWidget(releaseSpinBox);
            auto releaseLabel = new QLabel(tr("Release (ms)"));
            releaseLabel->setBuddy(releaseSpinBox);
            synthesizerLayout->addRow(releaseLabel, releaseLayout);
            
            auto synthesizerTestButton = new QPushButton(tr("Test"));
            synthesizerLayout->addRow(synthesizerTestButton);

            synthesizerGroupBox->setLayout(synthesizerLayout);
            mainLayout->addWidget(synthesizerGroupBox);

            auto controlGroupBox = new QGroupBox(tr("Control"));

            mainLayout->addWidget(controlGroupBox, 1);

            auto ms = AudioSystem::midiSystem();

            auto deviceList = talcs::MidiInputDevice::devices();
            if (!ms->device()) {
                deviceComboBox->addItem(tr("(Not working)"), -1);
            }
            for (int i = 0; i < deviceList.size(); i++) {
                deviceComboBox->addItem(deviceList.at(i), i);
                if (ms->device() && i == ms->device()->deviceIndex())
                    deviceComboBox->setCurrentIndex(i);
            }
            connect(deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
                auto i = deviceComboBox->itemData(index).toInt();
                if (i == -1)
                    return;
                if (!ms->setDevice(i)) {
                    QMessageBox::critical(this, {}, tr("Cannot open MIDI device %1").arg(deviceComboBox->itemText(index)));
                    QSignalBlocker o(deviceComboBox);
                    deviceComboBox->setCurrentIndex(deviceComboBox->findData(ms->device()->deviceIndex()));
                    if (auto notWorkingIndex = deviceComboBox->findData(-1); notWorkingIndex != -1)
                        deviceComboBox->removeItem(notWorkingIndex);

                }
            });
            connect(amplitudeSlider, &QSlider::valueChanged, this, [=](int value) {
                QSignalBlocker o(amplitudeSpinBox);
                amplitudeSpinBox->setValue(sliderValueToDecibel(value));
            });
            connect(amplitudeSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double decibel) {
                QSignalBlocker o(amplitudeSlider);
                amplitudeSlider->setValue(decibelToSliderValue(decibel));
            });
            connect(attackSlider, &QSlider::valueChanged, attackSpinBox, &QSpinBox::setValue);
            connect(attackSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), attackSlider, &QSlider::setValue);
            connect(releaseSlider, &QSlider::valueChanged, releaseSpinBox, &QSpinBox::setValue);
            connect(releaseSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), releaseSlider, &QSlider::setValue);

            generatorComboBox->setCurrentIndex(ms->generator());
            amplitudeSpinBox->setValue(ms->amplitudeDecibel());
            amplitudeSlider->setValue(decibelToSliderValue(amplitudeSpinBox->value()));
            attackSpinBox->setValue(ms->attackMsec());
            releaseSpinBox->setValue(ms->releaseMsec());

            setLayout(mainLayout);
        }

        ~MIDIPageWidget() override = default;

        void accept() {

        }


    };

    MIDIPage::MIDIPage(QObject *parent) : Core::ISettingPage("audio.MIDI", parent) {
        setTitle([] { return tr("MIDI"); });
        setDescription([] { return tr("Configure MIDI preferences."); });
    }
    MIDIPage::~MIDIPage() = default;

    QString MIDIPage::sortKeyword() const {
        return QStringLiteral("MIDI");
    }
    QWidget *MIDIPage::widget() {
        if (m_widget)
            return m_widget;
        return m_widget = new MIDIPageWidget;
    }
    bool MIDIPage::accept() {
        return false;
    }
    void MIDIPage::finish() {
    }
}

#include "midipage.moc"