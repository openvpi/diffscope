#ifndef TRACKENTITY_H
#define TRACKENTITY_H

#include <QJsonObject>

#include <qsubstate/vectorentity.h>

#include <dspxmodel/noteentity.h>
#include <dspxmodel/paramentity.h>

namespace QDspx {

    class DSPXMODEL_EXPORT ClipTimeEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
        Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
        Q_PROPERTY(int clipStart READ clipStart WRITE setClipStart NOTIFY clipStartChanged)
        Q_PROPERTY(int clipLength READ clipLength WRITE setClipLength NOTIFY clipLengthChanged)
    public:
        explicit ClipTimeEntity(QObject *parent = nullptr);
        ~ClipTimeEntity();

    public:
        int start() const;
        void setStart(int start);

        int length() const;
        void setLength(int length);

        int clipStart() const;
        void setClipStart(int clipStart);

        int clipLength() const;
        void setClipLength(int clipLength);

    Q_SIGNALS:
        void startChanged(int start);
        void lengthChanged(int length);
        void clipStartChanged(int clipStart);
        void clipLengthChanged(int clipLength);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        ClipTimeEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT ClipEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(QString clipName READ clipName WRITE setClipName NOTIFY clipNameChanged)
    public:
        ~ClipEntity();

    public:
        enum Type {
            Singing,
            Audio,
        };
        Q_ENUM(Type)

        Type type() const;

        QString clipName() const;
        void setClipName(const QString &clipName);

        ClipTimeEntity *time() const;
        BusControlEntity *control() const;

    Q_SIGNALS:
        void clipNameChanged(const QString &clipName);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        ClipEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT AudioClipEntity : public ClipEntity {
        Q_OBJECT
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    public:
        explicit AudioClipEntity(QObject *parent = nullptr);
        ~AudioClipEntity();

    public:
        QString path() const;
        void setPath(const QString &path);

    Q_SIGNALS:
        void pathChanged(const QString &path);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        AudioClipEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT SingingClipEntity : public ClipEntity {
        Q_OBJECT
    public:
        explicit SingingClipEntity(QObject *parent = nullptr);
        ~SingingClipEntity();

    public:
        ParamSetEntity *params() const;

        NoteListEntity *notes() const;

    protected:
        SingingClipEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    DSPXMODEL_DECLARE_SHEET_CLASS(ClipListEntity, ClipEntity)

    class DSPXMODEL_EXPORT TrackEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(QString trackName READ trackName WRITE setTrackName NOTIFY trackNameChanged)
        Q_PROPERTY(QJsonObject colorConfiguration READ colorConfiguration WRITE
                       setColorConfiguration NOTIFY colorConfigurationChanged)
    public:
        explicit TrackEntity(QObject *parent = nullptr);
        ~TrackEntity();

    public:
        QString trackName() const;
        void setTrackName(const QString &trackName);

        QJsonObject colorConfiguration() const;
        void setColorConfiguration(const QJsonObject &colorConfiguration);

        TrackControlEntity *control() const;
        ClipListEntity *clips() const;

    Q_SIGNALS:
        void trackNameChanged(const QString &trackName);
        void colorConfigurationChanged(const QJsonObject &colorConfiguration);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        TrackEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    DSPXMODEL_DECLARE_VECTOR_CLASS(TrackListEntity, TrackEntity)

}

#endif // TRACKENTITY_H
