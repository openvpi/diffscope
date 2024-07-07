#ifndef AUDIO_IAUDIO_H
#define AUDIO_IAUDIO_H

#include <audioglobal.h>

#include <QObject>

namespace Audio {

    namespace Internal {
        class AudioPlugin;
    }
    class OutputSystemInterface;

    class FormatManager;

    class IAudioPrivate;

    class AUDIO_EXPORT IAudio : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IAudio)
        friend class Internal::AudioPlugin;
    public:
        ~IAudio() override;
        static IAudio *instance();

        OutputSystemInterface *outputSystemInterface(bool isVST) const;

        FormatManager *formatManager() const;

    private:
        explicit IAudio(QObject *parent = nullptr);
        QScopedPointer<IAudioPrivate> d_ptr;
    };

}

#endif // AUDIO_IAUDIO_H
