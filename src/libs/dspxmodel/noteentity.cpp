#include "noteentity.h"

#include <substate/structnode.h>

#include "dspxconst.h"
#include "dspxmodelglobal_p.h"

using namespace Substate;

namespace QDspx {

    enum PhonemeSubscript {
        PS_Type,
        PS_Token,
        PS_Start,
    };

    PhonemeEntity::PhonemeEntity(QObject *parent)
        : PhonemeEntity(tagged_node<StructNode>("dspx_phoneme", 3), true, parent) {
    }

    PhonemeEntity::~PhonemeEntity() = default;

    PhonemeEntity::Type PhonemeEntity::type() const {
        return static_cast<Type>(valueImpl(PS_Type).variant->toInt());
    }

    void PhonemeEntity::setType(PhonemeEntity::Type type) {
        setVariantImpl(PS_Type, static_cast<int>(type));
    }

    QString PhonemeEntity::token() const {
        return valueImpl(PS_Token).variant->toString();
    }

    void PhonemeEntity::setToken(const QString &token) {
        setVariantImpl(PS_Token, token);
    }

    int PhonemeEntity::start() const {
        return valueImpl(PS_Start).variant->toInt();
    }

    void PhonemeEntity::setStart(int start) {
        setVariantImpl(PS_Start, start);
    }

    void PhonemeEntity::sendAssigned(int index, const Value &val, const Value &oldVal) {
        switch (index) {
            case PS_Type:
                Q_EMIT typeChanged(static_cast<Type>(val.variant->toInt()));
                return;
            case PS_Token:
                Q_EMIT tokenChanged(val.variant->toString());
                return;
            case PS_Start:
                Q_EMIT startChanged(val.variant->toInt());
                return;
            default:
                break;
        }
    }

    PhonemeEntity::PhonemeEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setType(Ahead);
            setToken(DefaultPhonemeToken);
            setStart(DefaultPhonemeStart);
        }
    }

    DSPXMODEL_DECLARE_VECTOR_CLASS_IMPL(PhonemeListEntity, "dspx_phoneme_list")

    enum PhonemeInfoSubscript {
        PIS_Edited,
    };

    PhonemeInfoEntity::PhonemeInfoEntity(QObject *parent)
        : PhonemeInfoEntity(tagged_node<StructNode>("dspx_phoneme_info", 1), true, parent) {
    }

    PhonemeInfoEntity::~PhonemeInfoEntity() = default;

    QJsonArray PhonemeInfoEntity::original() const {
        return dynamicDataImpl("dspx_phoneme_org").toJsonArray();
    }

    void PhonemeInfoEntity::setOriginal(const QJsonArray &original) {
        setDynamicDataImpl("dspx_phoneme_org", original);
        emit originalChanged(original);
    }

    PhonemeListEntity *PhonemeInfoEntity::edited() const {
        return static_cast<PhonemeListEntity *>(valueImpl(PIS_Edited).item);
    }

    PhonemeInfoEntity::PhonemeInfoEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setOriginal({});
            setValueImpl(PIS_Edited, new PhonemeListEntity());
        }
    }

    enum VibratoInfoSubscript {
        VIS_Start,
        VIS_End,
        VIS_Frequency,
        VIS_Phase,
        VIS_Amplitude,
        VIS_Offset,
        VIS_Points,
    };

    VibratoInfoEntity::VibratoInfoEntity(QObject *parent)
        : VibratoInfoEntity(tagged_node<StructNode>("dspx_vibrato_info", 7), true, parent) {
    }

    VibratoInfoEntity::~VibratoInfoEntity() = default;

    double VibratoInfoEntity::start() const {
        return valueImpl(VIS_Start).variant->toDouble();
    }

    void VibratoInfoEntity::setStart(double start) {
        setVariantImpl(VIS_Start, start);
    }

    double VibratoInfoEntity::end() const {
        return valueImpl(VIS_End).variant->toDouble();
    }

    void VibratoInfoEntity::setEnd(double end) {
        setVariantImpl(VIS_End, end);
    }

    double VibratoInfoEntity::frequency() const {
        return valueImpl(VIS_Frequency).variant->toDouble();
    }

    void VibratoInfoEntity::setFrequency(double frequency) {
        setVariantImpl(VIS_Frequency, frequency);
    }

    double VibratoInfoEntity::phase() const {
        return valueImpl(VIS_Phase).variant->toDouble();
    }

    void VibratoInfoEntity::setPhase(double phase) {
        setVariantImpl(VIS_Phase, phase);
    }

    double VibratoInfoEntity::amplitude() const {
        return valueImpl(VIS_Amplitude).variant->toDouble();
    }

    void VibratoInfoEntity::setAmplitude(double amplitude) {
        setVariantImpl(VIS_Amplitude, amplitude);
    }

    double VibratoInfoEntity::offset() const {
        return valueImpl(VIS_Offset).variant->toDouble();
    }

    void VibratoInfoEntity::setOffset(double offset) {
        setVariantImpl(VIS_Offset, offset);
    }

    DoublePointListEntity *VibratoInfoEntity::points() const {
        return static_cast<DoublePointListEntity *>(valueImpl(VIS_Points).item);
    }

    void VibratoInfoEntity::sendAssigned(int index, const Entity::Value &val,
                                         const Entity::Value &oldVal) {
        switch (index) {
            case VIS_Start:
                Q_EMIT startChanged(val.variant->toDouble());
                return;
            case VIS_End:
                Q_EMIT endChanged(val.variant->toDouble());
                return;
            case VIS_Frequency:
                Q_EMIT frequencyChanged(val.variant->toDouble());
                return;
            case VIS_Phase:
                Q_EMIT phaseChanged(val.variant->toDouble());
                return;
            case VIS_Amplitude:
                Q_EMIT amplitudeChanged(val.variant->toDouble());
                return;
            case VIS_Offset:
                Q_EMIT offsetChanged(val.variant->toDouble());
                return;
            default:
                break;
        }
    }

    VibratoInfoEntity::VibratoInfoEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setStart(DefaultVibratoStart);
            setEnd(DefaultVibratoEnd);
            setFrequency(DefaultVibratoFrequency);
            setPhase(DefaultVibratoPhase);
            setAmplitude(DefaultVibratoAmplitude);
            setOffset(DefaultVibratoOffset);
            setValueImpl(VIS_Points, new DoublePointListEntity());
        }
    }

    enum NoteSubscript {
        NS_Position,
        NS_Length,
        NS_KeyNumber,
        NS_Lyric,
        NS_OrgPronunciation,
        NS_Pronunciation,
        NS_PhonemeInfo,
        NS_VibratoInfo,
    };

    NoteEntity::NoteEntity(QObject *parent)
        : NoteEntity(tagged_node<StructNode>("dspx_note", 7), true, parent) {
    }

    NoteEntity::~NoteEntity() = default;

    int NoteEntity::position() const {
        return valueImpl(NS_Position).variant->toInt();
    }

    void NoteEntity::setPosition(int position) {
        setVariantImpl(NS_Position, position);
    }

    int NoteEntity::length() const {
        return valueImpl(NS_Length).variant->toInt();
    }

    void NoteEntity::setLength(int length) {
        setVariantImpl(NS_Length, length);
    }

    int NoteEntity::keyNumber() const {
        return valueImpl(NS_KeyNumber).variant->toInt();
    }

    void NoteEntity::setKeyNumber(int keyNumber) {
        setVariantImpl(NS_KeyNumber, keyNumber);
    }

    QString NoteEntity::lyric() const {
        return valueImpl(NS_Lyric).variant->toString();
    }

    void NoteEntity::setLyric(const QString &lyric) {
        setVariantImpl(NS_Lyric, lyric);
    }

    QString NoteEntity::orgPronunciation() const {
        return valueImpl(NS_OrgPronunciation).variant->toString();
    }

    void NoteEntity::setOrgPronunciation(const QString &orgPronunciation) {
        setVariantImpl(NS_OrgPronunciation, orgPronunciation);
    }

    QString NoteEntity::pronunciation() const {
        return valueImpl(NS_Pronunciation).variant->toString();
    }

    void NoteEntity::setPronunciation(const QString &pronunciation) {
        setVariantImpl(NS_Pronunciation, pronunciation);
    }

    PhonemeInfoEntity *NoteEntity::phonemeInfo() const {
        return static_cast<PhonemeInfoEntity *>(valueImpl(NS_PhonemeInfo).item);
    }

    VibratoInfoEntity *NoteEntity::vibratoInfo() const {
        return static_cast<VibratoInfoEntity *>(valueImpl(NS_VibratoInfo).item);
    }

    void NoteEntity::sendAssigned(int index, const Entity::Value &val,
                                  const Entity::Value &oldVal) {
        switch (index) {
            case NS_Position:
                Q_EMIT positionChanged(val.variant->toInt());
                return;
            case NS_Length:
                Q_EMIT lengthChanged(val.variant->toInt());
                return;
            case NS_KeyNumber:
                Q_EMIT keyNumberChanged(val.variant->toInt());
                return;
            case NS_Lyric:
                Q_EMIT lyricChanged(val.variant->toString());
                return;
            case NS_OrgPronunciation:
                Q_EMIT orgPronunciationChanged(val.variant->toString());
                return;
            case NS_Pronunciation:
                Q_EMIT pronunciationChanged(val.variant->toString());
                return;
            default:
                break;
        }
    }

    NoteEntity::NoteEntity(Node *node, bool init, QObject *parent)
        : StructEntityBase(node, parent) {
        if (init) {
            setPosition(DefaultNotePos);
            setLength(DefaultNoteLength);
            setKeyNumber(DefaultNoteKeyNum);
            setLyric(DefaultNoteLyric);
            setPronunciation(DefaultNotePronunciation);
            setValueImpl(NS_PhonemeInfo, new PhonemeInfoEntity());
            setValueImpl(NS_VibratoInfo, new VibratoInfoEntity());
        }
    }

    DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(NoteListEntity, "dspx_note_list")

}