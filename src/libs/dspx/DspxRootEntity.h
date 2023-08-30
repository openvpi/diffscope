#ifndef DSPXROOTENTITY_H
#define DSPXROOTENTITY_H

#include "DspxBaseEntity.h"
#include "DspxTimelineEntity.h"
#include "DspxTrackEntity.h"

namespace Dspx {

    class DspxContentEntity;
    class DspxFileMetaEntity;
    class DspxMasterEntity;

    //===========================================================================
    // Content
    class DSPXMODEL_EXPORT DspxContentEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxContentEntity(QObject *parent = nullptr);
        ~DspxContentEntity();

    public:
        DspxFileMetaEntity *metadata() const;
        DspxMasterEntity *master() const;
        DspxTimelineEntity *timeline() const;
        DspxTrackListEntity *tracks() const;
    };
    //===========================================================================

    //===========================================================================
    // FileMeta
    class DSPXMODEL_EXPORT DspxFileMetaEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxFileMetaEntity(QObject *parent = nullptr);
        ~DspxFileMetaEntity();

    public:
        QString author() const;
        void setAuthor(const QString &author);

        QString projectName() const;
        void setProjectName(const QString &projectName);

    Q_SIGNALS:
        void authorChanged(const QString &author);
        void projectNameChanged(const QString &projectName);
    };
    //===========================================================================

    //===========================================================================
    // Master
    class DSPXMODEL_EXPORT DspxMasterEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxMasterEntity(QObject *parent = nullptr);
        ~DspxMasterEntity();

    public:
        DspxBusControlEntity *control() const;
    };
    //===========================================================================

}

#endif // DSPXROOTENTITY_H
