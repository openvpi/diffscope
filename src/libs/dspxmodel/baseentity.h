#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <QPoint>

#include <qsubstate/sheetentity.h>
#include <qsubstate/structentity.h>

#include <dspxmodel/dspxmodelglobal.h>

namespace QDspx {

    namespace Private {
        class Initializer;
    }

    class DSPXMODEL_EXPORT BusControlEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(double gain READ gain WRITE setGain NOTIFY gainChanged)
        Q_PROPERTY(double pan READ pan WRITE setPan NOTIFY panChanged)
        Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    public:
        explicit BusControlEntity(QObject *parent = nullptr);
        ~BusControlEntity();

    public:
        double gain() const;
        void setGain(double gain);

        double pan() const;
        void setPan(double pan);

        bool mute() const;
        void setMute(bool mute);

    Q_SIGNALS:
        void gainChanged(double gain);
        void panChanged(double pan);
        void muteChanged(bool mute);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        BusControlEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT TrackControlEntity : public BusControlEntity {
        Q_OBJECT
        Q_PROPERTY(bool solo READ solo WRITE setSolo NOTIFY soloChanged)
    public:
        explicit TrackControlEntity(QObject *parent = nullptr);
        ~TrackControlEntity();

    public:
        bool solo() const;
        void setSolo(bool solo);

    Q_SIGNALS:
        void soloChanged(bool solo);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        TrackControlEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT IntPointEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int x READ x)
        Q_PROPERTY(int y READ y)
        Q_PROPERTY(QPoint pos READ pos WRITE setPos NOTIFY positionChanged)
    public:
        explicit IntPointEntity(QObject *parent = nullptr);
        ~IntPointEntity();

    public:
        int x() const;
        int y() const;

        QPoint pos() const;
        void setPos(const QPoint &pos);

    Q_SIGNALS:
        void positionChanged(const QPoint &pos);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        IntPointEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT DoublePointEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(double x READ x)
        Q_PROPERTY(double y READ y)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY positionChanged)
    public:
        explicit DoublePointEntity(QObject *parent = nullptr);
        ~DoublePointEntity();

    public:
        double x() const;
        double y() const;

        QPointF pos() const;
        void setPos(const QPointF &pos);

    Q_SIGNALS:
        void positionChanged(const QPointF &pos);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        DoublePointEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT AnchorPointEntity : public IntPointEntity {
        Q_OBJECT
        Q_PROPERTY(Interpolation interp READ interp WRITE setInterp NOTIFY interpChanged)
    public:
        explicit AnchorPointEntity(QObject *parent = nullptr);
        ~AnchorPointEntity();

    public:
        enum Interpolation {
            None,
            Linear,
            Hermite,
        };
        Q_ENUM(Interpolation)

        Interpolation interp() const;
        void setInterp(AnchorPointEntity::Interpolation i);

    Q_SIGNALS:
        void interpChanged(Interpolation i);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        AnchorPointEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    DSPXMODEL_DECLARE_SHEET_CLASS(IntPointListEntity, IntPointEntity)
    DSPXMODEL_DECLARE_SHEET_CLASS(DoublePointListEntity, DoublePointEntity)
    DSPXMODEL_DECLARE_SHEET_CLASS(AnchorPointListEntity, AnchorPointEntity)

}

#endif // BASEENTITY_H
