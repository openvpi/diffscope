#include "timelineentity.h"

#include <substate/structnode.h>
#include <substate/sheetnode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

using namespace Substate;

namespace QDspx {
    enum TimeSignatureSubscript {
        TSS_BarIndex,
        TSS_Value,
    };

    TimeSignatureEntity::TimeSignatureEntity(QObject *parent)
        : TimeSignatureEntity(tagged_node<StructNode>("dspx_time_signature", 2), true, parent) {
    }
    TimeSignatureEntity::~TimeSignatureEntity() = default;
    int TimeSignatureEntity::barIndex() const {
        return valueImpl(TSS_BarIndex).variant->toInt();
    }
    void TimeSignatureEntity::setBarIndex(int barIndex) {
        setVariantImpl(TSS_BarIndex, barIndex);
    }
    int TimeSignatureEntity::numerator() const {
        return value().numerator();
    }
    int TimeSignatureEntity::denominator() const {
        return value().denominator();
    }
    SVS::MusicTimeSignature TimeSignatureEntity::value() const {
        return valueImpl(TSS_Value).variant->value<SVS::MusicTimeSignature>();
    }
    void TimeSignatureEntity::setValue(const SVS::MusicTimeSignature &value) {
        setVariantImpl(TSS_Value, QVariant::fromValue(value));
    }
    void TimeSignatureEntity::sendAssigned(int index, const Entity::Value &val,
                                           const Entity::Value &oldVal) {
        switch (index) {
            case TSS_BarIndex:
                Q_EMIT barIndexChanged(val.variant->toInt());
                return;
            case TSS_Value:
                Q_EMIT valueChanged(val.variant->value<SVS::MusicTimeSignature>());
                return;
            default:
                break;
        }
    }
    TimeSignatureEntity::TimeSignatureEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setBarIndex(DefaultTimeSignatureBarIndex);
            setValue({DefaultTimeSignatureNumerator, DefaultTimeSignatureDenominator});
        }
    }

    enum TempoSubscript {
        TS_Position,
        TS_Value,
    };

    TempoEntity::TempoEntity(QObject *parent)
        : TempoEntity(tagged_node<StructNode>("dspx_tempo", 2), true, parent) {
    }
    TempoEntity::~TempoEntity() = default;
    int TempoEntity::position() const {
        return valueImpl(TS_Position).variant->toInt();
    }
    void TempoEntity::setPosition(int position) {
        setVariantImpl(TS_Position, position);
    }
    double TempoEntity::value() const {
        return valueImpl(TS_Value).variant->toDouble();
    }
    void TempoEntity::setValue(double value) {
        setVariantImpl(TS_Value, value);
    }
    void TempoEntity::sendAssigned(int index, const Entity::Value &val,
                                   const Entity::Value &oldVal) {
        switch (index) {
            case TS_Position:
                Q_EMIT positionChanged(val.variant->toInt());
                return;
            case TS_Value:
                Q_EMIT valueChanged(val.variant->toDouble());
                return;
            default:
                break;
        }
    }
    TempoEntity::TempoEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setPosition(DefaultTempoPos);
            setValue(DefaultTempoValue);
        }
    }

    enum TimelineLabelSubscript {
        TLS_Position,
        TLS_Text,
    };

    TimelineLabelEntity::TimelineLabelEntity(QObject *parent)
        : TimelineLabelEntity(tagged_node<StructNode>("dspx_timeline_label", 2), true, parent) {
    }
    TimelineLabelEntity::~TimelineLabelEntity() = default;
    int TimelineLabelEntity::position() const {
        return valueImpl(TLS_Position).variant->toInt();
    }
    void TimelineLabelEntity::setPosition(int position) {
        setVariantImpl(TLS_Position, position);
    }
    QString TimelineLabelEntity::text() const {
        return valueImpl(TLS_Text).variant->toString();
    }
    void TimelineLabelEntity::setText(const QString &text) {
        setVariantImpl(TLS_Text, text);
    }
    void TimelineLabelEntity::sendAssigned(int index, const Entity::Value &val,
                                           const Entity::Value &oldVal) {
        switch (index) {
            case TLS_Position:
                Q_EMIT positionChanged(val.variant->toInt());
                return;
            case TLS_Text:
                Q_EMIT textChanged(val.variant->toString());
                return;
            default:
                break;
        }
    }
    TimelineLabelEntity::TimelineLabelEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setPosition(DefaultLabelPos);
            setText(DefaultLabelText);
        }
    }

    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(TimeSignatureListEntity, "dspx_time_signature_list")
    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(TempoListEntity, "dspx_tempo_list")
    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(TimelineLabelListEntity, "dspx_timeline_label_list")

    enum TimelineSubscript {
        TS_TimeSignatures,
        TS_Tempos,
        TS_Labels,
    };

    TimelineEntity::TimelineEntity(QObject *parent)
        : TimelineEntity(tagged_node<StructNode>("dspx_timeline", 3), true, parent) {
    }

    TimelineEntity::~TimelineEntity() = default;

    TimeSignatureListEntity *TimelineEntity::timeSignatures() const {
        return static_cast<TimeSignatureListEntity *>(valueImpl(TS_TimeSignatures).item);
    }

    TempoListEntity *TimelineEntity::tempos() const {
        return static_cast<TempoListEntity *>(valueImpl(TS_Tempos).item);
    }

    TimelineLabelListEntity *TimelineEntity::labels() const {
        return static_cast<TimelineLabelListEntity *>(valueImpl(TS_Labels).item);
    }

    TimelineEntity::TimelineEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setValueImpl(TS_TimeSignatures, new TimeSignatureListEntity());
            setValueImpl(TS_Tempos, new TempoListEntity());
            setValueImpl(TS_Labels, new TimelineLabelListEntity());
        }
    }

}