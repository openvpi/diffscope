#ifndef AUDIO_AUDIOSYSTEM_H
#define AUDIO_AUDIOSYSTEM_H

#include <QObject>

namespace Audio {

    class OutputSystem;
    class VSTConnectionSystem;

    class AudioSystem : public QObject {
        Q_OBJECT
    public:
        explicit AudioSystem(QObject *parent = nullptr);
        ~AudioSystem() override;
        static AudioSystem *instance();

        static OutputSystem *outputSystem();
        static VSTConnectionSystem *vstConnectionSystem();

    private:
        OutputSystem *m_outputSystem;
        VSTConnectionSystem *m_vstConnectionSystem;
    };

} // Audio

#endif // DIFFSCOPE_AUDIOSYSTEM_H
