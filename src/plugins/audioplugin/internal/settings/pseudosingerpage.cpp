#include "pseudosingerpage.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include <SVSCraftCore/decibellinearizer.h>
#include <SVSCraftWidgets/seekbar.h>
#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>

namespace Audio::Internal {

    class PseudoSingerPageWidget : public QWidget {
        Q_OBJECT
    public:
        explicit PseudoSingerPageWidget(QWidget *parent = nullptr) : QWidget(parent) {
            auto mainLayout = new QVBoxLayout;

            auto synthesizerGroupBox = new QGroupBox(tr("Synthesizer"));
            auto synthesizerMainLayout = new QVBoxLayout;

            auto synthesizerHeaderLayout = new QFormLayout;
            auto synthesizerSelectLayout = new QHBoxLayout;
            auto synthesizerSelectComboBox = new QComboBox;
            synthesizerSelectComboBox->addItems({"Synth 1", "Synth 2", "Synth 3", "Synth 4"});
            synthesizerSelectLayout->addWidget(synthesizerSelectComboBox, 1);
            auto synthesizerTestButton = new QPushButton(tr("&Preview"));
            synthesizerTestButton->setCheckable(true);
            synthesizerSelectLayout->addWidget(synthesizerTestButton);
            auto synthesizerSelectLabel = new QLabel(tr("&Synth No."));
            synthesizerSelectLabel->setBuddy(synthesizerSelectComboBox);
            synthesizerHeaderLayout->addRow(synthesizerSelectLabel, synthesizerSelectLayout);
            synthesizerMainLayout->addLayout(synthesizerHeaderLayout);

            auto synthesizerBodyLayout = new QHBoxLayout;
            synthesizerBodyLayout->addSpacing(32);
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
            decaySlider->setDefaultValue(10);
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
            decayRatioSlider->setDefaultValue(1);
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

            synthesizerBodyLayout->addLayout(synthesizerLayout);
            synthesizerMainLayout->addLayout(synthesizerBodyLayout);
            synthesizerGroupBox->setLayout(synthesizerMainLayout);
            mainLayout->addWidget(synthesizerGroupBox);

            auto parameterGroupBox = new QGroupBox(tr("Parameter"));
            auto parameterLayout = new QVBoxLayout;
            auto readPitchCheckBox = new QCheckBox(tr("Read pitc&h"));
            parameterLayout->addWidget(readPitchCheckBox);
            auto readEnergyCheckBox = new QCheckBox(tr("Read energ&y"));
            parameterLayout->addWidget(readEnergyCheckBox);
            parameterGroupBox->setLayout(parameterLayout);
            mainLayout->addWidget(parameterGroupBox);

            mainLayout->addStretch();

            setLayout(mainLayout);
        }
    };

    PseudoSingerPage::PseudoSingerPage(QObject *parent) : Core::ISettingPage("audio.PseudoSinger", parent) {
        setTitle([] { return tr("Pseudo Singer"); });
        setDescription([] { return tr("The pseudo singer is built into %1 for outputting synthesized audio. It allows you to preview a project without specifying a virtual singer.").arg(QApplication::applicationName()); });
    }
    PseudoSingerPage::~PseudoSingerPage() = default;

    QString PseudoSingerPage::sortKeyword() const {
        return QStringLiteral("PseudoSinger");
    }
    QWidget *PseudoSingerPage::widget() {
        if (m_widget)
            return m_widget;
        return m_widget = new PseudoSingerPageWidget;
    }
    bool PseudoSingerPage::accept() {
        return true;
    }
    void PseudoSingerPage::finish() {
    }

}

#include "pseudosingerpage.moc"