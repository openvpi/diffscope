#include "paramentity.h"

#include <substate/structnode.h>
#include <substate/sheetnode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

using namespace Substate;

namespace QDspx {

    enum ParamCurveSubscript {
        PCS_Type,
        PCS_Start,
    };

    ParamCurveEntity::~ParamCurveEntity() = default;

    ParamCurveEntity::Type ParamCurveEntity::type() const {
        return static_cast<Type>(valueImpl(PCS_Type).variant->toInt());
    }

    int ParamCurveEntity::start() const {
        return valueImpl(PCS_Start).variant->toInt();
    }

    void ParamCurveEntity::setStart(int start) {
        setVariantImpl(PCS_Start, start);
    }

    void ParamCurveEntity::sendAssigned(int index, const Entity::Value &val,
                                        const Entity::Value &oldVal) {
        switch (index) {
            case PCS_Start:
                Q_EMIT startChanged(val.variant->toInt());
                return;
            default:
                break;
        }
    }

    ParamCurveEntity::ParamCurveEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setStart(DefaultParamCurveStart);
        }
    }

    enum ParamFreeSubscript {
        PFS_Step = 2,
        PFS_Values,
    };

    ParamFreeEntity::ParamFreeEntity(QObject *parent)
        : ParamFreeEntity(tagged_node<StructNode>("dspx_param_free", 4), true, parent) {
    }

    ParamFreeEntity::~ParamFreeEntity() = default;

    int ParamFreeEntity::step() const {
        return valueImpl(PFS_Step).variant->toInt();
    }

    Int16ArrayEntity *ParamFreeEntity::values() const {
        return static_cast<Int16ArrayEntity *>(valueImpl(PFS_Values).item);
    }

    ParamFreeEntity::ParamFreeEntity(Node *node, bool init, QObject *parent)
        : ParamCurveEntity(node, init, parent) {
        if (init) {
            setVariantImpl(PCS_Type, int(Free));
            setVariantImpl(PFS_Step, DefaultParamFreeStep);
            setValueImpl(PFS_Values, new Int16ArrayEntity());
        }
    }

    enum ParamAnchorSubscript {
        PAS_Nodes = 2,
    };

    ParamAnchorEntity::ParamAnchorEntity(QObject *parent)
        : ParamAnchorEntity(tagged_node<StructNode>("dspx_param_anchor", 3), true, parent) {
    }

    ParamAnchorEntity::~ParamAnchorEntity() = default;

    AnchorPointListEntity *ParamAnchorEntity::nodes() const {
        return static_cast<AnchorPointListEntity *>(valueImpl(PAS_Nodes).item);
    }

    ParamAnchorEntity::ParamAnchorEntity(Node *node, bool init, QObject *parent)
        : ParamCurveEntity(node, init, parent) {
        if (init) {
            setVariantImpl(PCS_Type, int(Anchor));
            setValueImpl(PAS_Nodes, new AnchorPointListEntity());
        }
    }

    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(ParamCurveListEntity, "dspx_param_curve_list")

    enum ParamInfoSubscript {
        PIS_Edited,
        PIS_Envelope,
    };

    ParamInfoEntity::ParamInfoEntity(QObject *parent)
        : ParamInfoEntity(tagged_node<StructNode>("dspx_param_info", 1), true, parent) {
    }

    ParamInfoEntity::~ParamInfoEntity() = default;

    QJsonArray ParamInfoEntity::original() const {
        return dynamicDataImpl("dspx_param_org").toJsonArray();
    }

    void ParamInfoEntity::setOriginal(const QJsonArray &original) {
        setDynamicDataImpl("dspx_param_org", original);
        emit originalChanged(original);
    }

    ParamCurveListEntity *ParamInfoEntity::edited() const {
        return static_cast<ParamCurveListEntity *>(valueImpl(PIS_Edited).item);
    }

    ParamCurveListEntity *ParamInfoEntity::envelope() const {
        return static_cast<ParamCurveListEntity *>(valueImpl(PIS_Envelope).item);
    }

    ParamInfoEntity::ParamInfoEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setOriginal({});
            setValueImpl(PIS_Edited, new ParamCurveListEntity());
            setValueImpl(PIS_Envelope, new ParamCurveListEntity());
        }
    }

    enum ParamSetSubscript {
        PSS_Pitch,
        PSS_Energy,
        PSS_Breathiness,
        PSS_Tension,
    };

    ParamSetEntity::ParamSetEntity(QObject *parent)
        : ParamSetEntity(tagged_node<StructNode>("dspx_param_set", 4), true, parent) {
    }

    ParamSetEntity::~ParamSetEntity() {
    }

    ParamInfoEntity *ParamSetEntity::pitch() const {
        return static_cast<ParamInfoEntity *>(valueImpl(PSS_Pitch).item);
    }

    ParamInfoEntity *ParamSetEntity::energy() const {
        return static_cast<ParamInfoEntity *>(valueImpl(PSS_Energy).item);
    }

    ParamInfoEntity *ParamSetEntity::breathiness() const {
        return static_cast<ParamInfoEntity *>(valueImpl(PSS_Breathiness).item);
    }

    ParamInfoEntity *ParamSetEntity::tension() const {
        return static_cast<ParamInfoEntity *>(valueImpl(PSS_Tension).item);
    }

    ParamSetEntity::ParamSetEntity(Substate::Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setValueImpl(PSS_Pitch, new ParamInfoEntity());
            setValueImpl(PIS_Envelope, new ParamInfoEntity());
            setValueImpl(PSS_Breathiness, new ParamInfoEntity());
            setValueImpl(PSS_Tension, new ParamInfoEntity());
        }
    }

}