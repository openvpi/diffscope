#ifndef AUDIO_AUDIOSYSTEM_H
#define AUDIO_AUDIOSYSTEM_H

#include <QObject>

namespace Audio::Internal {

    class OutputSystem;
    class VSTConnectionSystem;
    class MidiSystem;

    class AudioSystem : public QObject {
        Q_OBJECT
    public:
        explicit AudioSystem(QObject *parent = nullptr);
        ~AudioSystem() override;
        static AudioSystem *instance();

        static OutputSystem *outputSystem();
        static VSTConnectionSystem *vstConnectionSystem();
        static MidiSystem *midiSystem();

    private:
        OutputSystem *m_outputSystem;
        VSTConnectionSystem *m_vstConnectionSystem;
        MidiSystem *m_midiSystem;
    };

}

#endif // DIFFSCOPE_AUDIOSYSTEM_H
