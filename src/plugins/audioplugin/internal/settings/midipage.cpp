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
#include <QMutex>
#include <QCheckBox>

#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>
#include <SVSCraftWidgets/seekbar.h>
#include <SVSCraftCore/decibellinearizer.h>

#include <TalcsCore/NoteSynthesizer.h>
#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AbstractOutputContext.h>
#include <TalcsMidi/MidiInputDevice.h>
#include <TalcsMidi/MidiMessage.h>
#include <TalcsMidi/MidiNoteSynthesizer.h>

#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/midisystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>
#include <audioplugin/internal/audiohelpers.h>
#include <audioplugin/internal/settingpagesynthhelper.h>

namespace Audio::Internal {

    class MIDIPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MIDIPageWidget(QWidget *parent = nullptr) : QWidget(parent), d(new SettingPageSynthHelper(this)) {
            auto mainLayout = new QVBoxLayout;

            auto inputGroupBox = new QGroupBox(tr("MIDI Input"));
            auto inputLayout = new QFormLayout;
            auto deviceComboBox = new QComboBox;
            inputLayout->addRow(tr("&Device"), deviceComboBox);
            inputGroupBox->setLayout(inputLayout);
            mainLayout->addWidget(inputGroupBox);

            auto synthesizerGroupBox = new QGroupBox(tr("Synthesizer"));
            auto synthesizerLayout = new QFormLayout;
            auto generatorComboBox = new QComboBox;
            generatorComboBox->addItems({tr("Sine wave"), tr("Square wave"), tr("Triangle Wave"), tr("Sawtooth wave")});
            synthesizerLayout->addRow(tr("&Generator"), generatorComboBox);

            auto amplitudeLayout = new QHBoxLayout;
            auto amplitudeSlider = new SVS::SeekBar;
            amplitudeSlider->setDefaultValue(SVS::DecibelLinearizer::decibelToLinearValue(-3));
            amplitudeSlider->setRange(SVS::DecibelLinearizer::decibelToLinearValue(-96), SVS::DecibelLinearizer::decibelToLinearValue(0));
            amplitudeLayout->addWidget(amplitudeSlider);
            auto amplitudeSpinBox = new SVS::ExpressionDoubleSpinBox;
            amplitudeSpinBox->setDecimals(1);
            amplitudeSpinBox->setRange(-96, 0);
            amplitudeSpinBox->setSpecialValueText("-INF");
            amplitudeLayout->addWidget(amplitudeSpinBox);
            auto amplitudeLabel = new QLabel(tr("&Amplitude (dB)"));
            amplitudeLabel->setBuddy(amplitudeSpinBox);
            synthesizerLayout->addRow(amplitudeLabel, amplitudeLayout);

            auto attackLayout = new QHBoxLayout;
            auto attackSlider = new SVS::SeekBar;
            attackSlider->setInterval(1);
            attackSlider->setDefaultValue(10);
            attackSlider->setRange(0, 100);
            attackLayout->addWidget(attackSlider);
            auto attackSpinBox = new SVS::ExpressionSpinBox;
            attackSpinBox->setRange(0, 100);
            attackLayout->addWidget(attackSpinBox);
            auto attackLabel = new QLabel(tr("A&ttack (ms)"));
            attackLabel->setBuddy(attackSpinBox);
            synthesizerLayout->addRow(attackLabel, attackLayout);
            
            auto decayLayout = new QHBoxLayout;
            auto decaySlider = new SVS::SeekBar;
            decaySlider->setInterval(1);
            decaySlider->setDefaultValue(1000);
            decaySlider->setRange(0, 1000);
            decayLayout->addWidget(decaySlider);
            auto decaySpinBox = new SVS::ExpressionSpinBox;
            decaySpinBox->setRange(0, 1000);
            decayLayout->addWidget(decaySpinBox);
            auto decayLabel = new QLabel(tr("D&ecay (ms)"));
            decayLabel->setBuddy(decaySpinBox);
            synthesizerLayout->addRow(decayLabel, decayLayout);
            
            auto decayRatioLayout = new QHBoxLayout;
            auto decayRatioSlider = new SVS::SeekBar;
            decayRatioSlider->setDefaultValue(0.5);
            decayRatioSlider->setRange(0, 1);
            decayRatioLayout->addWidget(decayRatioSlider);
            auto decayRatioSpinBox = new SVS::ExpressionDoubleSpinBox;
            decayRatioSpinBox->setRange(0, 1);
            decayRatioLayout->addWidget(decayRatioSpinBox);
            auto decayRatioLabel = new QLabel(tr("Decay rati&o"));
            decayRatioLabel->setBuddy(decayRatioSpinBox);
            synthesizerLayout->addRow(decayRatioLabel, decayRatioLayout);

            auto releaseLayout = new QHBoxLayout;
            auto releaseSlider = new SVS::SeekBar;
            releaseSlider->setInterval(1);
            releaseSlider->setDefaultValue(50);
            releaseSlider->setRange(0, 100);
            releaseLayout->addWidget(releaseSlider);
            auto releaseSpinBox = new SVS::ExpressionSpinBox;
            releaseSpinBox->setRange(0, 100);
            releaseLayout->addWidget(releaseSpinBox);
            auto releaseLabel = new QLabel(tr("&Release (ms)"));
            releaseLabel->setBuddy(releaseSpinBox);
            synthesizerLayout->addRow(releaseLabel, releaseLayout);

            auto frequencyOfALayout = new QVBoxLayout;
            auto frequencyOfASpinBox = new SVS::ExpressionDoubleSpinBox;
            frequencyOfASpinBox->setRange(440.0 * std::pow(2, -1.0/24.0), 440.0 * std::pow(2, 1.0/24.0));
            frequencyOfALayout->addWidget(frequencyOfASpinBox);
            auto adjustByProjectCheckBox = new QCheckBox(tr("Ad&just by the cent shift of the active project window"));
            frequencyOfALayout->addWidget(adjustByProjectCheckBox);
            auto frequencyOfALabel = new QLabel(tr("&Frequency of A"));
            frequencyOfALabel->setBuddy(frequencyOfASpinBox);
            synthesizerLayout->addRow(frequencyOfALabel, frequencyOfALayout);

            auto synthesizerButtonLayout = new QHBoxLayout;
            auto synthesizerTestButton = new QPushButton(tr("&Preview"));
            synthesizerTestButton->setCheckable(true);
            synthesizerButtonLayout->addWidget(synthesizerTestButton);
            auto flushButton = new QPushButton(tr("&Interrupt All Notes"));
            flushButton->setToolTip(tr("Interrupt all notes that are currently played by the synthesizer"));
            synthesizerButtonLayout->addWidget(flushButton);
            synthesizerButtonLayout->addStretch();
            synthesizerLayout->addRow(synthesizerButtonLayout);

            synthesizerGroupBox->setLayout(synthesizerLayout);
            mainLayout->addWidget(synthesizerGroupBox);
            setLayout(mainLayout);

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
                    deviceComboBox->setCurrentIndex(deviceComboBox->findData(ms->device() ? ms->device()->deviceIndex() : -1));
                } else {
                    QSignalBlocker o(deviceComboBox);
                    if (auto notWorkingIndex = deviceComboBox->findData(-1); notWorkingIndex != -1)
                        deviceComboBox->removeItem(notWorkingIndex);
                }
            });

            d->m_cachedGenerator = ms->generator();
            d->m_cachedAmplitude = ms->amplitudeDecibel();
            d->m_cachedAttackMsec = ms->attackMsec();
            d->m_cachedDecayMsec = ms->decayMsec();
            d->m_cachedDecayRatio = ms->decayRatio();
            d->m_cachedReleaseMsec = ms->releaseMsec();
            d->m_cachedFrequencyOfA = ms->frequencyOfA();

            d->initialize(generatorComboBox, amplitudeSlider, amplitudeSpinBox, attackSlider, attackSpinBox, decaySlider, decaySpinBox, decayRatioSlider, decayRatioSpinBox, releaseSlider, releaseSpinBox, synthesizerTestButton);

            connect(frequencyOfASpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
                d->m_mutex.lock();
                if (!adjustByProjectCheckBox->isChecked())
                    d->m_cachedFrequencyOfA = value;
                d->m_mutex.unlock();
                d->m_testSynthesizer.flush();
            });
            connect(adjustByProjectCheckBox, &QAbstractButton::clicked, this, [=](bool checked) {
                d->m_mutex.lock();
                if (checked) {
                    d->m_cachedFrequencyOfA = 0;
                    frequencyOfASpinBox->setDisabled(true);
                } else {
                    d->m_cachedFrequencyOfA = frequencyOfASpinBox->value();
                    frequencyOfASpinBox->setDisabled(false);
                }
                d->m_mutex.unlock();
                d->m_testSynthesizer.flush();
            });

            d->m_cachedFrequencyOfA = ms->frequencyOfA();
            if (qFuzzyIsNull(d->m_cachedFrequencyOfA)) {
                adjustByProjectCheckBox->setChecked(true);
                frequencyOfASpinBox->setDisabled(true);
                frequencyOfASpinBox->setValue(440.0);
            } else {
                frequencyOfASpinBox->setValue(d->m_cachedFrequencyOfA);
            }

            connect(synthesizerTestButton, &QAbstractButton::clicked, this, [=](bool checked) {
                if (checked) {
                    if (!AudioSystem::outputSystem()->isReady()) {
                        synthesizerTestButton->setChecked(false);
                        return;
                    }
                }
                d->toggleTestState(checked);
            });
            connect(d, &SettingPageSynthHelper::testFinished, this, [=] {
                synthesizerTestButton->setChecked(false);
            }, Qt::QueuedConnection);
            connect(flushButton, &QAbstractButton::clicked, this, [=] {
                AudioSystem::midiSystem()->synthesizer()->noteSynthesizer()->flush();
                AudioSystem::vstConnectionSystem()->synthesizer()->noteSynthesizer()->flush();
            });
        }

        ~MIDIPageWidget() override = default;

        void accept() const {
            auto ms = AudioSystem::midiSystem();
            ms->setGenerator(d->m_cachedGenerator);
            ms->setAmplitudeDecibel(d->m_cachedAmplitude);
            ms->setAttackMsec(d->m_cachedAttackMsec);
            ms->setDecayMsec(d->m_cachedDecayMsec);
            ms->setDecayRatio(d->m_cachedDecayRatio);
            ms->setReleaseMsec(d->m_cachedReleaseMsec);
            ms->setFrequencyOfA(d->m_cachedFrequencyOfA);
            AudioSystem::vstConnectionSystem()->syncSynthesizerPreference();
        }

        SettingPageSynthHelper *d;

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
        if (m_widget)
            m_widget->accept();
        return true;
    }
    void MIDIPage::finish() {
    }
}

#include "midipage.moc"