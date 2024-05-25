#ifndef PARAMENTITY_H
#define PARAMENTITY_H

#include <QJsonArray>

#include <qsubstate/vectorentity.h>
#include <qsubstate/arrayentity.h>

#include <dspxmodel/baseentity.h>

namespace QDspx {

    class DSPXMODEL_EXPORT ParamCurveEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
    public:
        ~ParamCurveEntity();

        enum Type {
            Anchor,
            Free,
        };
        Q_ENUM(Type)

        Type type() const;

    public:
        int start() const;
        void setStart(int start);

    Q_SIGNALS:
        void startChanged(int start);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        ParamCurveEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT ParamFreeEntity : public ParamCurveEntity {
        Q_OBJECT
    public:
        explicit ParamFreeEntity(QObject *parent = nullptr);
        ~ParamFreeEntity();

    public:
        int step() const;
        Substate::Int16ArrayEntity *values() const;

    protected:
        ParamFreeEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT ParamAnchorEntity : public ParamCurveEntity {
        Q_OBJECT
    public:
        explicit ParamAnchorEntity(QObject *parent = nullptr);
        ~ParamAnchorEntity();

    public:
        AnchorPointListEntity *nodes() const;

    protected:
        ParamAnchorEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    DSPXMODEL_DECLARE_SHEET_CLASS(ParamCurveListEntity, ParamCurveEntity)

    class DSPXMODEL_EXPORT ParamInfoEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(QJsonArray original READ original WRITE setOriginal NOTIFY originalChanged)
    public:
        explicit ParamInfoEntity(QObject *parent = nullptr);
        ~ParamInfoEntity();

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        ParamCurveListEntity *edited() const;

        ParamCurveListEntity *envelope() const;

    Q_SIGNALS:
        void originalChanged(const QJsonArray &org);

    protected:
        ParamInfoEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT ParamSetEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit ParamSetEntity(QObject *parent = nullptr);
        ~ParamSetEntity();

    public:
        ParamInfoEntity *pitch() const;

        ParamInfoEntity *energy() const;

        ParamInfoEntity *breathiness() const;

        ParamInfoEntity *tension() const;

    protected:
        ParamSetEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

}

#endif // PARAMENTITY_H
