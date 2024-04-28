#include "midipage.h"

namespace Audio {
    MIDIPage::MIDIPage(QObject *parent) : Core::ISettingPage("audio.MIDI", parent) {
    }
    MIDIPage::~MIDIPage() {
    }
    QString MIDIPage::sortKeyword() const {
        return ISettingPage::sortKeyword();
    }
    QWidget *MIDIPage::widget() {
        return nullptr;
    }
    bool MIDIPage::accept() {
        return false;
    }
    void MIDIPage::finish() {
    }
}