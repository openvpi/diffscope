#include "settingpagesynthhelper.h"

#include <QComboBox>
#include <QRandomGenerator>
#include <QCoreApplication>

#include <TalcsCore/Decibels.h>
#include <TalcsMidi/MidiMessage.h>

#include <SVSCraftCore/decibellinearizer.h>
#include <SVSCraftWidgets/seekbar.h>
#include <SVSCraftWidgets/expressionspinbox.h>
#include <SVSCraftWidgets/expressiondoublespinbox.h>

#include <audioplugin/internal/audiohelpers.h>
#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>

namespace Audio::Internal {

    static talcs::NoteSynthesizerDetectorMessage scores[] = {
        {0, {47, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {0, {75, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {2, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {2, {66, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {4, {66, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {4, {73, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {6, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {6, {71, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {8, {71, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {8, {66, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {10, {66, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {10, {73, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {12, {47, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {12, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},

        {12, {49, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {12, {75, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {14, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {14, {66, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {15, {66, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {15, {76, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {16, {76, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {16, {75, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {18, {49, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {18, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {18, {42, .5, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {18, {73, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {24, {42, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {24, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        talcs::NoteSynthesizerDetectorMessage::Null,
    };

    static talcs::NoteSynthesizerDetectorMessage alternativeScores[] = {
        {0, {59, .3, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {0, {66, .3, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {0, {75, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {4, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {4, {73, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {6, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {6, {71, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {10, {71, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {10, {73, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {12, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {12, {59, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {12, {66, talcs::NoteSynthesizerDetectorMessage::NoteOff}},

        {12, {52, .3, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {12, {59, .3, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {12, {75, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {15, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {15, {76, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {16, {76, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {16, {75, .7, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {18, {75, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {18, {73, 1, talcs::NoteSynthesizerDetectorMessage::NoteOn}},
        {24, {73, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {24, {52, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        {24, {59, talcs::NoteSynthesizerDetectorMessage::NoteOff}},
        talcs::NoteSynthesizerDetectorMessage::Null,
    };

    /**
     * Just for fun :)
     */
    static struct {
        bool hit = false;
        bool enabled = false;

        void handlePreviewStarted() {
            if (hit) {
                enabled = !enabled;
            } else {
                hit = true;
            }
        }

        void handlePreviewStopped() {
            hit = false;
        }

        const talcs::NoteSynthesizerDetectorMessage *selectScores() const {
            if (enabled) {
                return QRandomGenerator::global()->bounded(0, 2) ? scores : alternativeScores;
            } else {
                return scores;
            }
        }

        const QString selectTooltip() const {
            if (enabled) {
                return QRandomGenerator::global()->bounded(0, 2) ? QCoreApplication::translate("Audio::Internal::SettingPageSynthHelper", "Why did you play ...?!") : QCoreApplication::translate("Audio::Internal::SettingPageSynthHelper", "I'll put an end to it.");
            } else {
                return {};
            }
        }
    } easterEggHelper;

    SettingPageSynthHelper::SettingPageSynthHelper(QObject *parent) : QObject(parent), m_adoptedScores(scores) {

    }
    SettingPageSynthHelper::~SettingPageSynthHelper() {
        m_testMixer.removeAllSources();
        AudioSystem::outputSystem()->context()->preMixer()->removeSource(&m_testMixer);
    }
    void SettingPageSynthHelper::initialize(
        QComboBox *generatorComboBox, SVS::SeekBar *amplitudeSlider,
        SVS::ExpressionDoubleSpinBox *amplitudeSpinBox, SVS::SeekBar *attackSlider,
        SVS::ExpressionSpinBox *attackSpinBox, SVS::SeekBar *decaySlider,
        SVS::ExpressionSpinBox *decaySpinBox, SVS::SeekBar *decayRatioSlider,
        SVS::ExpressionDoubleSpinBox *decayRatioSpinBox, SVS::SeekBar *releaseSlider,
        SVS::ExpressionSpinBox *releaseSpinBox, QWidget *previewButton) {

        connect(generatorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
            m_cachedGenerator = index;
            m_testSynthesizer.setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(index));
        });
        connect(amplitudeSlider, &SVS::SeekBar::valueChanged, this, [=](double value) {
            amplitudeSpinBox->setValue(SVS::DecibelLinearizer::linearValueToDecibel(value));
        });
        connect(amplitudeSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double decibel) {
            QSignalBlocker o(amplitudeSlider);
            m_cachedAmplitude = decibel;
            amplitudeSlider->setValue(SVS::DecibelLinearizer::decibelToLinearValue(decibel));
            m_testMixer.setGain(talcs::Decibels::decibelsToGain(decibel));
        });
        connect(attackSlider, &SVS::SeekBar::valueChanged, attackSpinBox, &QSpinBox::setValue);
        connect(attackSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            QSignalBlocker o(attackSlider);
            m_cachedAttackMsec = value;
            attackSlider->setValue(value);
            m_testSynthesizer.setAttackTime(AudioHelpers::msecToSample(value, m_testSynthesizer.sampleRate()));
        });
        connect(decaySlider, &SVS::SeekBar::valueChanged, decaySpinBox, &QSpinBox::setValue);
        connect(decaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            QSignalBlocker o(decaySlider);
            m_cachedDecayMsec = value;
            decaySlider->setValue(value);
            m_testSynthesizer.setDecayTime(AudioHelpers::msecToSample(value, m_testSynthesizer.sampleRate()));
        });
        connect(decayRatioSlider, &SVS::SeekBar::valueChanged, decayRatioSpinBox, &QDoubleSpinBox::setValue);
        connect(decayRatioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            QSignalBlocker o(decayRatioSlider);
            m_cachedDecayRatio = value;
            decayRatioSlider->setValue(value);
            m_testSynthesizer.setDecayRatio(value);
        });
        connect(releaseSlider, &SVS::SeekBar::valueChanged, releaseSpinBox, &QSpinBox::setValue);
        connect(releaseSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            QSignalBlocker o(decayRatioSlider);
            m_cachedReleaseMsec = value;
            releaseSlider->setValue(value);
            m_testSynthesizer.setReleaseTime(AudioHelpers::msecToSample(value, m_testSynthesizer.sampleRate()));
        });
        connect(AudioSystem::outputSystem()->context(), &talcs::AbstractOutputContext::sampleRateChanged, this, [=](double sampleRate) {
            m_testSynthesizer.setAttackTime(AudioHelpers::msecToSample(attackSpinBox->value(), sampleRate));
            m_testSynthesizer.setReleaseTime(AudioHelpers::msecToSample(releaseSpinBox->value(), sampleRate));
        });

        generatorComboBox->setCurrentIndex(m_cachedGenerator);
        amplitudeSpinBox->setValue(m_cachedAmplitude);
        attackSpinBox->setValue(m_cachedAttackMsec);
        decaySpinBox->setValue(m_cachedDecayMsec);
        decayRatioSpinBox->setValue(m_cachedDecayRatio);
        releaseSpinBox->setValue(m_cachedReleaseMsec);

        m_testSynthesizer.setDetector(this);
        m_testMixer.addSource(&m_testSynthesizer);
        AudioSystem::outputSystem()->context()->preMixer()->addSource(&m_testMixer);
        m_testSynthesizer.setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(m_cachedGenerator));
        m_testMixer.setGain(talcs::Decibels::decibelsToGain(m_cachedAmplitude));
        m_testSynthesizer.setAttackTime(AudioHelpers::msecToSample(m_cachedAttackMsec, m_testSynthesizer.sampleRate()));
        m_testSynthesizer.setDecayTime(AudioHelpers::msecToSample(m_cachedDecayMsec, m_testSynthesizer.sampleRate()));
        m_testSynthesizer.setDecayRatio(m_cachedDecayRatio);
        m_testSynthesizer.setReleaseTime(AudioHelpers::msecToSample(m_cachedReleaseMsec, m_testSynthesizer.sampleRate()));

        connect(this, &SettingPageSynthHelper::testFinished, previewButton, [=] {
            previewButton->setToolTip(easterEggHelper.selectTooltip());
        });
    }
    void SettingPageSynthHelper::detectInterval(qint64 intervalLength) {
        QMutexLocker locker(&m_mutex);
        if (m_currentScoreIndex == -2) {
            m_currentScoreIndex = -3;
            return;
        }
        if (m_currentScoreIndex == -1 && !isTestFinished) {
            isTestFinished = true;
            easterEggHelper.handlePreviewStopped();
            emit testFinished();
            return;
        }
        m_currentInterval = 97.0 / 60.0 * 4.0 * static_cast<double>(intervalLength) / m_testMixer.sampleRate();
        m_currentPosition += m_currentInterval;
    }
    talcs::NoteSynthesizerDetectorMessage SettingPageSynthHelper::nextMessage() {
        QMutexLocker locker(&m_mutex);
        if (m_currentScoreIndex == -3) {
            m_currentScoreIndex = -1;
            return {0, talcs::NoteSynthesizerDetectorMessage::AllNotesOff};
        } else if (m_currentScoreIndex < 0) {
            return talcs::NoteSynthesizerDetectorMessage::Null;
        }
        auto message = m_adoptedScores[m_currentScoreIndex];
        if (message.position == -1) {
            m_currentScoreIndex = -1;
            return talcs::NoteSynthesizerDetectorMessage::Null;
        }
        if (static_cast<double>(message.position) > m_currentPosition) {
            return talcs::NoteSynthesizerDetectorMessage::Null;
        }
        message.position = static_cast<qint64>(std::round((static_cast<double>(message.position)  - (m_currentPosition - m_currentInterval)) * m_testMixer.sampleRate() / (97.0 / 60.0 * 4.0)));
        message.note.frequency = talcs::MidiMessage::getMidiNoteInHertz(static_cast<int>(message.note.frequency), qFuzzyIsNull(m_cachedFrequencyOfA) ? 440.0 : m_cachedFrequencyOfA);
        m_currentScoreIndex++;
        return message;
    }
    void SettingPageSynthHelper::toggleTestState(bool enabled) {
        QMutexLocker locker(&m_mutex);
        if (enabled) {
            m_adoptedScores = easterEggHelper.selectScores();
            easterEggHelper.handlePreviewStarted();
            isTestFinished = false;
            m_currentPosition = 0;
            m_currentScoreIndex = 0;
        } else {
            m_currentScoreIndex = -2;
        }
    }
}