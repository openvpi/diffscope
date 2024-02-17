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
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT FileMetaEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit FileMetaEntity(QObject *parent = nullptr);
        ~FileMetaEntity();

    public:
        QString author() const;
        void setAuthor(const QString &author);

        QString projectName() const;
        void setProjectName(const QString &projectName);

    Q_SIGNALS:
        void authorChanged(const QString &author);
        void projectNameChanged(const QString &projectName);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        FileMetaEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT ModelEntity : public Substate::StructEntityBase {
        Q_OBJECT
    public:
        explicit ModelEntity(QObject *parent = nullptr);
        ~ModelEntity();

    public:
        FileMetaEntity *metadata() const;
        MasterEntity *master() const;
        TimelineEntity *timeline() const;
        TrackListEntity *tracks() const;

    protected:
        ModelEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

}

#endif // MODELENTITY_H
