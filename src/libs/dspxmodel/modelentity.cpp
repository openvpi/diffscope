#include "modelentity.h"

#include <substate/structnode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

using namespace Substate;

namespace QDspx {

    // REGISTER_FACTORY.*\(.+(\".+\")\)
    // Entity::removeFactory($1);

#define REGISTER_FACTORY(Class, Key)                                                               \
    Entity::registerFactory(Key, [](Node *node, QObject *parent) -> Entity * {                     \
        return new Class(node, false, parent);                                                     \
    });

#define REGISTER_FACTORY_CONTAINER(Class, Key)                                                     \
    Entity::registerFactory(                                                                       \
        Key, [](Node *node, QObject *parent) -> Entity * { return new Class(node, parent); });

    struct DspxInitializer {
        DspxInitializer() {
            REGISTER_FACTORY(BusControlEntity, "dspx_bus_control")
            REGISTER_FACTORY(TrackControlEntity, "dspx_track_control")
            REGISTER_FACTORY(IntPointEntity, "dspx_int_point")
            REGISTER_FACTORY(DoublePointEntity, "dspx_double_point")
            REGISTER_FACTORY(AnchorPointEntity, "dspx_anchor_point")
            REGISTER_FACTORY_CONTAINER(IntPointListEntity, "dspx_int_point_list")
            REGISTER_FACTORY_CONTAINER(DoublePointListEntity, "dspx_double_point_list")
            REGISTER_FACTORY_CONTAINER(AnchorPointListEntity, "dspx_anchor_point_list")

            REGISTER_FACTORY(PhonemeEntity, "dspx_phoneme")
            REGISTER_FACTORY_CONTAINER(PhonemeListEntity, "dspx_phoneme_list")
            REGISTER_FACTORY(PhonemeInfoEntity, "dspx_phoneme_info")
            REGISTER_FACTORY(VibratoInfoEntity, "dspx_vibrato_info")
            REGISTER_FACTORY(NoteEntity, "dspx_note")
            REGISTER_FACTORY_CONTAINER(NoteListEntity, "dspx_note_list")

            REGISTER_FACTORY(ParamFreeEntity, "dspx_param_free")
            REGISTER_FACTORY(ParamAnchorEntity, "dspx_param_anchor")
            REGISTER_FACTORY_CONTAINER(ParamCurveListEntity, "dspx_param_curve_list")
            REGISTER_FACTORY(ParamInfoEntity, "dspx_param_info")
            REGISTER_FACTORY(ParamSetEntity, "dspx_param_set")

            REGISTER_FACTORY(TimeSignatureEntity, "dspx_time_signature")
            REGISTER_FACTORY(TempoEntity, "dspx_tempo")
            REGISTER_FACTORY(TimelineLabelEntity, "dspx_timeline_label")
            REGISTER_FACTORY_CONTAINER(TimeSignatureListEntity, "dspx_time_signature_list")
            REGISTER_FACTORY_CONTAINER(TempoListEntity, "dspx_tempo_list")
            REGISTER_FACTORY_CONTAINER(TimelineLabelListEntity, "dspx_timeline_label_list")

            REGISTER_FACTORY(ClipTimeEntity, "dspx_clip_time")
            REGISTER_FACTORY(AudioClipEntity, "dspx_audio_clip")
            REGISTER_FACTORY(SingingClipEntity, "dspx_singing_clip")
            REGISTER_FACTORY_CONTAINER(ClipListEntity, "dspx_clip_list")
            REGISTER_FACTORY(TrackEntity, "dspx_track")
            REGISTER_FACTORY_CONTAINER(TrackListEntity, "dspx_track_list")

            REGISTER_FACTORY(MasterEntity, "dspx_master")
            REGISTER_FACTORY(FileGlobalEntity, "dspx_file_meta")
            REGISTER_FACTORY(ModelEntity, "dspx_model")
        }

        ~DspxInitializer() {
            Entity::removeFactory("dspx_bus_control");
            Entity::removeFactory("dspx_track_control");
            Entity::removeFactory("dspx_int_point");
            Entity::removeFactory("dspx_double_point");
            Entity::removeFactory("dspx_anchor_point");
            Entity::removeFactory("dspx_int_point_list");
            Entity::removeFactory("dspx_double_point_list");
            Entity::removeFactory("dspx_anchor_point_list");

            Entity::removeFactory("dspx_phoneme");
            Entity::removeFactory("dspx_phoneme_list");
            Entity::removeFactory("dspx_phoneme_info");
            Entity::removeFactory("dspx_vibrato_info");
            Entity::removeFactory("dspx_note");
            Entity::removeFactory("dspx_note_list");

            Entity::removeFactory("dspx_param_free");
            Entity::removeFactory("dspx_param_anchor");
            Entity::removeFactory("dspx_param_curve_list");
            Entity::removeFactory("dspx_param_info");
            Entity::removeFactory("dspx_param_set");

            Entity::removeFactory("dspx_time_signature");
            Entity::removeFactory("dspx_tempo");
            Entity::removeFactory("dspx_timeline_label");
            Entity::removeFactory("dspx_time_signature_list");
            Entity::removeFactory("dspx_tempo_list");
            Entity::removeFactory("dspx_timeline_label_list");

            Entity::removeFactory("dspx_clip_time");
            Entity::removeFactory("dspx_audio_clip");
            Entity::removeFactory("dspx_singing_clip");
            Entity::removeFactory("dspx_clip_list");
            Entity::removeFactory("dspx_track");
            Entity::removeFactory("dspx_track_list");

            Entity::removeFactory("dspx_master");
            Entity::removeFactory("dspx_file_meta");
            Entity::removeFactory("dspx_model");
        }
    } initializer;

    enum FileMetaSubscript {
        FMS_Author,
        FMS_ProjectName,
        FMS_CentShift,
    };

    FileGlobalEntity::FileGlobalEntity(QObject *parent)
        : FileGlobalEntity(tagged_node<StructNode>("dspx_file_meta", 4), true, parent) {
    }
    FileGlobalEntity::~FileGlobalEntity() = default;
    QString FileGlobalEntity::author() const {
        return valueImpl(FMS_Author).variant->toString();
    }
    void FileGlobalEntity::setAuthor(const QString &author) {
        setVariantImpl(FMS_Author, author);
    }
    QString FileGlobalEntity::projectName() const {
        return valueImpl(FMS_ProjectName).variant->toString();
    }
    void FileGlobalEntity::setProjectName(const QString &projectName) {
        setVariantImpl(FMS_ProjectName, projectName);
    }
    int FileGlobalEntity::centShift() const {
        return valueImpl(FMS_CentShift).variant->toInt();
    }
    void FileGlobalEntity::setCentShift(int centShift) {
        setVariantImpl(FMS_CentShift, centShift);
    }
    void FileGlobalEntity::sendAssigned(int index, const Entity::Value &val,
                                        const Entity::Value &oldVal) {
        switch (index) {
            case FMS_Author:
                Q_EMIT authorChanged(val.variant->toString());
                return;
            case FMS_ProjectName:
                Q_EMIT projectNameChanged(val.variant->toString());
                return;
            case FMS_CentShift:
                Q_EMIT centShiftChanged(val.variant->toInt());
                return;
            default:
                break;
        }
    }
    FileGlobalEntity::FileGlobalEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setAuthor(DefaultAuthor);
            setProjectName(DefaultProjectName);
            setCentShift(DefaultCentShift);
        }
    }

    enum MasterSubscript {
        MS_Control,
    };

    MasterEntity::MasterEntity(QObject *parent)
        : MasterEntity(tagged_node<StructNode>("dspx_master", 1), true, parent) {
    }
    MasterEntity::~MasterEntity() = default;
    BusControlEntity *MasterEntity::control() const {
        return static_cast<BusControlEntity *>(valueImpl(MS_Control).item);
    }
    MasterEntity::MasterEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setValueImpl(MS_Control, new BusControlEntity());
        }
    }

    enum ModelSubscript {
        MS_Global,
        MS_Master,
        MS_Timeline,
        MS_Track,
    };

    ModelEntity::ModelEntity(QObject *parent)
        : ModelEntity(tagged_node<StructNode>("dspx_model", 4), true, parent) {
    }
    ModelEntity::~ModelEntity() = default;
    FileGlobalEntity *ModelEntity::global() const {
        return static_cast<FileGlobalEntity *>(valueImpl(MS_Global).item);
    }
    MasterEntity *ModelEntity::master() const {
        return static_cast<MasterEntity *>(valueImpl(MS_Master).item);
    }
    TimelineEntity *ModelEntity::timeline() const {
        return static_cast<TimelineEntity *>(valueImpl(MS_Timeline).item);
    }
    TrackListEntity *ModelEntity::tracks() const {
        return static_cast<TrackListEntity *>(valueImpl(MS_Track).item);
    }
    ModelEntity::ModelEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setValueImpl(MS_Global, new FileGlobalEntity());
            setValueImpl(MS_Master, new MasterEntity());
            setValueImpl(MS_Timeline, new TimelineEntity());
            setValueImpl(MS_Track, new TrackListEntity());
        }
    }

}