#include "baseentity.h"

#include <substate/structnode.h>
#include <substate/sheetnode.h>
#include <substate/vectornode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

#include "trackentity.h"

using namespace Substate;

namespace QDspx {

    enum ClipTimeSubscript {
        CTS_Start,
        CTS_Length,
        CTS_ClipStart,
        CTS_ClipLength,
    };

    ClipTimeEntity::ClipTimeEntity(QObject *parent)
        : ClipTimeEntity(tagged_node<StructNode>("dspx_clip_time", 4), true, parent) {
    }
    ClipTimeEntity::~ClipTimeEntity() = default;
    int ClipTimeEntity::start() const {
        return valueImpl(CTS_Start).variant->toInt();
    }
    void ClipTimeEntity::setStart(int start) {
        setVariantImpl(CTS_Start, start);
    }
    int ClipTimeEntity::length() const {
        return valueImpl(CTS_Length).variant->toInt();
    }
    void ClipTimeEntity::setLength(int length) {
        setVariantImpl(CTS_Length, length);
    }
    int ClipTimeEntity::clipStart() const {
        return valueImpl(CTS_ClipStart).variant->toInt();
    }
    void ClipTimeEntity::setClipStart(int clipStart) {
        setVariantImpl(CTS_ClipStart, clipStart);
    }
    int ClipTimeEntity::clipLength() const {
        return valueImpl(CTS_ClipLength).variant->toInt();
    }
    void ClipTimeEntity::setClipLength(int clipLength) {
        setVariantImpl(CTS_ClipLength, clipLength);
    }
    void ClipTimeEntity::sendAssigned(int index, const Entity::Value &val,
                                      const Entity::Value &oldVal) {
        switch (index) {
            case CTS_Start:
                Q_EMIT startChanged(val.variant->toInt());
                return;
            case CTS_Length:
                Q_EMIT lengthChanged(val.variant->toInt());
                return;
            case CTS_ClipStart:
                Q_EMIT clipStartChanged(val.variant->toInt());
                return;
            case CTS_ClipLength:
                Q_EMIT clipLengthChanged(val.variant->toInt());
                return;
            default:
                break;
        }
    }

    ClipTimeEntity::ClipTimeEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
    }

    enum ClipSubscript {
        CS_Type,
        CS_ClipName,
        CS_Time,
        CS_Control,
    };

    ClipEntity::~ClipEntity() = default;

    ClipEntity::Type ClipEntity::type() const {
        return static_cast<Type>(valueImpl(CS_Type).variant->toInt());
    }

    QString ClipEntity::clipName() const {
        return valueImpl(CS_ClipName).variant->toString();
    }

    void ClipEntity::setClipName(const QString &clipName) {
        setVariantImpl(CTS_ClipStart, clipName);
    }

    ClipTimeEntity *ClipEntity::time() const {
        return static_cast<ClipTimeEntity *>(valueImpl(CS_Time).item);
    }

    BusControlEntity *ClipEntity::control() const {
        return static_cast<BusControlEntity *>(valueImpl(CS_Control).item);
    }

    void ClipEntity::sendAssigned(int index, const Entity::Value &val,
                                  const Entity::Value &oldVal) {
        switch (index) {
            case CS_ClipName:
                Q_EMIT clipNameChanged(val.variant->toString());
                return;
            default:
                break;
        }
    }

    ClipEntity::ClipEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setClipName(DefaultClipName);
            setValueImpl(CS_Time, new ClipTimeEntity());
            setValueImpl(CS_Control, new BusControlEntity());
        }
    }

    enum AudioClipSubscript {
        ACS_Path = 4,
    };

    AudioClipEntity::AudioClipEntity(QObject *parent)
        : AudioClipEntity(tagged_node<StructNode>("dspx_audio_clip", 5), true, parent) {
    }

    AudioClipEntity::~AudioClipEntity() = default;

    QString AudioClipEntity::path() const {
        return valueImpl(ACS_Path).variant->toString();
    }

    void AudioClipEntity::setPath(const QString &path) {
        setVariantImpl(ACS_Path, path);
    }

    void AudioClipEntity::sendAssigned(int index, const Entity::Value &val,
                                       const Entity::Value &oldVal) {
        switch (index) {
            case ACS_Path: {
                Q_EMIT pathChanged(val.variant->toString());
                return;
            }
            default:
                break;
        }
        ClipEntity::sendAssigned(index, val, oldVal);
    }

    AudioClipEntity::AudioClipEntity(Node *node, bool init, QObject *parent)
        : ClipEntity(node, init, parent) {
        if (init) {
            setVariantImpl(CS_Type, int(Audio));
            setPath(DefaultAudioClipPath);
        }
    }

    enum SingingClipSubscript {
        SCS_Params = 4,
        SCS_Notes,
    };

    SingingClipEntity::SingingClipEntity(QObject *parent)
        : SingingClipEntity(tagged_node<StructNode>("dspx_singing_clip", 6), true, parent) {
    }

    SingingClipEntity::~SingingClipEntity() = default;

    ParamSetEntity *SingingClipEntity::params() const {
        return static_cast<ParamSetEntity *>(valueImpl(SCS_Params).item);
    }

    NoteListEntity *SingingClipEntity::notes() const {
        return static_cast<NoteListEntity *>(valueImpl(SCS_Notes).item);
    }

    SingingClipEntity::SingingClipEntity(Node *node, bool init, QObject *parent)
        : ClipEntity(node, init, parent) {
        if (init) {
            setVariantImpl(CS_Type, int(Singing));
            setValueImpl(SCS_Params, new ParamSetEntity());
            setValueImpl(SCS_Notes, new NoteListEntity());
        }
    }

    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(ClipListEntity, "dspx_clip_list")

    enum TrackSubscript {
        TS_TrackName,
        TS_ColorConfiguration,
        TS_Control,
        TS_Clips,
    };

    TrackEntity::TrackEntity(QObject *parent)
        : TrackEntity(tagged_node<StructNode>("dspx_track", 4), true, parent) {
    }
    TrackEntity::~TrackEntity() = default;
    QString TrackEntity::trackName() const {
        return valueImpl(TS_TrackName).variant->toString();
    }
    void TrackEntity::setTrackName(const QString &trackName) {
        setVariantImpl(TS_TrackName, trackName);
    }
    QJsonObject TrackEntity::colorConfiguration() const {
        return valueImpl(TS_ColorConfiguration).variant->toJsonObject();
    }
    void TrackEntity::setColorConfiguration(const QJsonObject &colorConfiguration) {
        setVariantImpl(TS_ColorConfiguration, colorConfiguration);
    }
    TrackControlEntity *TrackEntity::control() const {
        return static_cast<TrackControlEntity *>(valueImpl(TS_Control).item);
    }
    ClipListEntity *TrackEntity::clips() const {
        return static_cast<ClipListEntity *>(valueImpl(TS_Clips).item);
    }
    void TrackEntity::sendAssigned(int index, const Entity::Value &val,
                                   const Entity::Value &oldVal) {
        switch (index) {
            case TS_TrackName: {
                Q_EMIT trackNameChanged(val.variant->toString());
                return;
            }
            case TS_ColorConfiguration: {
                Q_EMIT colorConfigurationChanged(val.variant->toJsonObject());
                return;
            }
            default:
                break;
        }
    }
    TrackEntity::TrackEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setTrackName(DefaultTrackName);
            setColorConfiguration({});
            setValueImpl(TS_Control, new TrackControlEntity());
            setValueImpl(TS_Clips, new ClipListEntity());
        }
    }

    DSPXMODEL_DECLARE_VECTOR_CLASS_IMPL(TrackListEntity, "dspx_track_list")

}