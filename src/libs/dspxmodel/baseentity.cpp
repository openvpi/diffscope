#include "baseentity.h"

#include <substate/structnode.h>
#include <substate/sheetnode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

using namespace Substate;

namespace QDspx {

    enum BusControlSubscript {
        BCS_Gain,
        BCS_Pan,
        BCS_Mute,
    };

    BusControlEntity::BusControlEntity(QObject *parent)
        : BusControlEntity(tagged_node<StructNode>("dspx_bus_control", 3), true, parent) {
    }

    BusControlEntity::~BusControlEntity() = default;

    double BusControlEntity::gain() const {
        return valueImpl(BCS_Gain).variant->toDouble();
    }

    void BusControlEntity::setGain(double gain) {
        setVariantImpl(BCS_Gain, gain);
    }

    double BusControlEntity::pan() const {
        return valueImpl(BCS_Pan).variant->toDouble();
    }

    void BusControlEntity::setPan(double gain) {
        setVariantImpl(BCS_Pan, gain);
    }

    bool BusControlEntity::mute() const {
        return valueImpl(BCS_Mute).variant->toBool();
    }

    void BusControlEntity::setMute(bool mute) {
        setVariantImpl(BCS_Mute, mute);
    }

    void BusControlEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case BCS_Gain:
                Q_EMIT gainChanged(val.variant->toDouble());
                return;
            case BCS_Pan:
                Q_EMIT panChanged(val.variant->toDouble());
                return;
            case BCS_Mute:
                Q_EMIT muteChanged(val.variant->toBool());
                return;
            default:
                break;
        }
    }

    BusControlEntity::BusControlEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setGain(DefaultGain);
            setPan(DefaultPan);
            setMute(DefaultMute);
        }
    }

    enum TrackControlSubscript {
        TCS_Solo = 3,
    };

    TrackControlEntity::TrackControlEntity(QObject *parent)
        : TrackControlEntity(tagged_node<StructNode>("dspx_track_control", 4), true, parent) {
    }

    TrackControlEntity::~TrackControlEntity() = default;

    bool TrackControlEntity::solo() const {
        return valueImpl(TCS_Solo).variant->toBool();
    }

    void TrackControlEntity::setSolo(bool solo) {
        setVariantImpl(TCS_Solo, solo);
    }

    void TrackControlEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case TCS_Solo:
                Q_EMIT soloChanged(val.variant->toBool());
                return;
            default:
                break;
        }
        BusControlEntity::sendAssigned(index, val, oldVal);
    }

    TrackControlEntity::TrackControlEntity(Node *node, bool init, QObject *parent)
        : BusControlEntity(node, init, parent) {
        if (init) {
            setSolo(DefaultSolo);
        }
    }

    enum IntPointSubscript {
        IPS_Point = 0,
    };

    IntPointEntity::IntPointEntity(QObject *parent)
        : IntPointEntity(tagged_node<StructNode>("dspx_int_point", 1), true, parent) {
    }

    IntPointEntity::~IntPointEntity() = default;

    int IntPointEntity::x() const {
        return pos().x();
    }

    int IntPointEntity::y() const {
        return pos().y();
    }

    QPoint IntPointEntity::pos() const {
        return valueImpl(IPS_Point).variant->toPoint();
    }

    void IntPointEntity::setPos(const QPoint &pos) {
        setVariantImpl(IPS_Point, pos);
    }

    void IntPointEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case IPS_Point:
                Q_EMIT positionChanged(val.variant->toPoint());
                return;
            default:
                break;
        }
    }

    IntPointEntity::IntPointEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setPos(QPoint());
        }
    }

    enum DoublePointSubscript {
        DPS_Point = 0,
    };

    DoublePointEntity::DoublePointEntity(QObject *parent)
        : DoublePointEntity(tagged_node<StructNode>("dspx_double_point", 1), true, parent) {
    }

    DoublePointEntity::~DoublePointEntity() = default;

    double DoublePointEntity::x() const {
        return pos().x();
    }

    double DoublePointEntity::y() const {
        return pos().y();
    }

    QPointF DoublePointEntity::pos() const {
        return valueImpl(DPS_Point).variant->toPointF();
    }

    void DoublePointEntity::setPos(const QPointF &pos) {
        setVariantImpl(DPS_Point, pos);
    }

    void DoublePointEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case DPS_Point:
                Q_EMIT positionChanged(val.variant->toPointF());
                return;
            default:
                break;
        }
    }

    DoublePointEntity::DoublePointEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setPos(QPointF());
        }
    }

    enum AnchorPointSubscript {
        APS_Interpolation = 1,
    };

    AnchorPointEntity::AnchorPointEntity(QObject *parent)
        : AnchorPointEntity(tagged_node<StructNode>("dspx_anchor_point", 2), true, parent) {
    }

    AnchorPointEntity::~AnchorPointEntity() = default;

    AnchorPointEntity::Interpolation AnchorPointEntity::interp() const {
        return static_cast<Interpolation>(valueImpl(APS_Interpolation).variant->toInt());
    }

    void AnchorPointEntity::setInterp(Interpolation i) {
        setVariantImpl(APS_Interpolation, static_cast<int>(i));
    }

    void AnchorPointEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case APS_Interpolation:
                Q_EMIT interpChanged(static_cast<Interpolation>(val.variant->toInt()));
                return;
            default:
                break;
        }
        IntPointEntity::sendAssigned(index, val, oldVal);
    }

    AnchorPointEntity::AnchorPointEntity(Node *node, bool init, QObject *parent)
        : IntPointEntity(node, init, parent) {
        if (init) {
            setInterp(None);
        }
    }

    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(IntPointListEntity, "dspx_int_point_list")
    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(DoublePointListEntity, "dspx_double_point_list")
    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(AnchorPointListEntity, "dspx_anchor_point_list")

}