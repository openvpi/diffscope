#include "midipage.h"

#include <QBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QSlider>

#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>
#include <QLabel>

namespace Audio::Internal {

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
            // TODO
            amplitudeLayout->addWidget(amplitudeSlider);
            auto amplitudeSpinBox = new SVS::ExpressionDoubleSpinBox;
            // TODO
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

            synthesizerGroupBox->setLayout(synthesizerLayout);
            mainLayout->addWidget(synthesizerGroupBox);

            auto controlGroupBox = new QGroupBox(tr("Control"));

            mainLayout->addWidget(controlGroupBox, 1);

            setLayout(mainLayout);
        }

        ~MIDIPageWidget() override = default;
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