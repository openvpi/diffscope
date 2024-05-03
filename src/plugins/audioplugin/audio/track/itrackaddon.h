#ifndef AUDIO_ITRACKADDON_H
#define AUDIO_ITRACKADDON_H

#include <CoreApi/iexecutive.h>

#include <audioplugin/audioglobal.h>

namespace Audio {

    class TrackInterface;

    class AUDIO_EXPORT ITrackAddOn : Core::IExecutiveAddOn {
        Q_OBJECT
        friend class Core::IExecutiveRegistry<TrackInterface>;
    public:
        ~ITrackAddOn() override;

        TrackInterface *trackInterface() const;

    protected:
        explicit ITrackAddOn(QObject *parent = nullptr);
    };

}

#endif // AUDIO_ITRACKADDON_H
