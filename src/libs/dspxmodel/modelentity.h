#ifndef MODELENTITY_H
#define MODELENTITY_H

#include <dspxmodel/noteentity.h>
#include <dspxmodel/paramentity.h>
#include <dspxmodel/trackentity.h>
#include <dspxmodel/timelineentity.h>

namespace QDspx {

    class DSPXMODEL_EXPORT MasterEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit MasterEntity(QObject *parent = nullptr);
        ~MasterEntity();

    public:
        BusControlEntity *control() const;

    protected:
        MasterEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT FileGlobalEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
        Q_PROPERTY(
            QString projectName READ projectName WRITE setProjectName NOTIFY projectNameChanged)
        Q_PROPERTY(int centShift READ centShift WRITE setCentShift NOTIFY centShiftChanged)
    public:
        explicit FileGlobalEntity(QObject *parent = nullptr);
        ~FileGlobalEntity();

    public:
        QString author() const;
        void setAuthor(const QString &author);

        QString projectName() const;
        void setProjectName(const QString &projectName);

        int centShift() const;
        void setCentShift(int centShift);

    Q_SIGNALS:
        void authorChanged(const QString &author);
        void projectNameChanged(const QString &projectName);
        void centShiftChanged(int centShift);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        FileGlobalEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

    class DSPXMODEL_EXPORT ModelEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit ModelEntity(QObject *parent = nullptr);
        ~ModelEntity();

    public:
        FileGlobalEntity *global() const;
        MasterEntity *master() const;
        TimelineEntity *timeline() const;
        TrackListEntity *tracks() const;

    public:
        ModelEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class Private::Initializer;
    };

}

#endif // MODELENTITY_H
