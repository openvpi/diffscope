#include "audiopage.h"

namespace Audio::Internal {

    AudioPage::AudioPage(QObject *parent)
        : Core::ISettingPage(QStringLiteral("audio.Audio"), parent) {
        setTitle([] { return tr("Audio"); });
        setDescription([] { return tr("Configure Audio Preferences."); });
    }

    AudioPage::~AudioPage() = default;

    QString AudioPage::sortKeyword() const {
        return "Audio";
    }

    QWidget *AudioPage::widget() {
        return nullptr;
    }

    bool AudioPage::accept() {
        return true;
    }

    void AudioPage::finish() {
    }
}