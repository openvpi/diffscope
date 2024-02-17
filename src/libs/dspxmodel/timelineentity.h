#ifndef TIMELINEENTITY_H
#define TIMELINEENTITY_H

#include <SVSCraftCore/musictimeline.h>
#include <qsubstate/vectorentity.h>

#include <dspxmodel/baseentity.h>

namespace QDspx {

    class DSPXMODEL_EXPORT TimeSignatureEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int barIndex READ barIndex WRITE setBarIndex NOTIFY barIndexChanged)
        Q_PROPERTY(SVS::MusicTimeSignature value READ value WRITE setValue NOTIFY valueChanged)
        Q_PROPERTY(int numerator READ numerator)
        Q_PROPERTY(int denominator READ denominator)
    public:
        explicit TimeSignatureEntity(QObject *parent = nullptr);
        ~TimeSignatureEntity();

    public:
        int barIndex() const;
        void setBarIndex(int barIndex);

        int numerator() const;
        int denominator() const;

        SVS::MusicTimeSignature value() const;
        void setValue(const SVS::MusicTimeSignature &value);

    Q_SIGNALS:
        void barIndexChanged(int barIndex);
        void valueChanged(const SVS::MusicTimeSignature &value);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        TimeSignatureEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT TempoEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    public:
        explicit TempoEntity(QObject *parent = nullptr);
        ~TempoEntity();

    public:
        int position() const;
        void setPosition(int position);

        double value() const;
        void setValue(double value);

    Q_SIGNALS:
        void positionChanged(int position);
        void valueChanged(double value);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        TempoEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT TimelineLabelEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    public:
        explicit TimelineLabelEntity(QObject *parent = nullptr);
        ~TimelineLabelEntity();

    public:
        int position() const;
        void setPosition(int position);

        QString text() const;
        void setText(const QString &text);

    Q_SIGNALS:
        void positionChanged(int position);
        void textChanged(const QString &text);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        TimelineLabelEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    DSPXMODEL_DECLARE_SHEET_CLASS(TimeSignatureListEntity, TimeSignatureEntity)
    DSPXMODEL_DECLARE_SHEET_CLASS(TempoListEntity, TempoEntity)
    DSPXMODEL_DECLARE_SHEET_CLASS(TimelineLabelListEntity, TimelineLabelEntity)

    class DSPXMODEL_EXPORT TimelineEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit TimelineEntity(QObject *parent = nullptr);
        ~TimelineEntity();

    public:
        TimeSignatureListEntity *timeSignatures() const;
        TempoListEntity *tempos() const;
        TimelineLabelListEntity *labels() const;

    protected:
        TimelineEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

}

#endif // TIMELINEENTITY_H
