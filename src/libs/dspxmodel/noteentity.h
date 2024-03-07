#ifndef NOTEENTITY_H
#define NOTEENTITY_H

#include <QJsonArray>

#include <qsubstate/vectorentity.h>

#include <dspxmodel/baseentity.h>

namespace QDspx {

    class DSPXMODEL_EXPORT PhonemeEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(PhonemeEntity::Type type READ type WRITE setType NOTIFY typeChanged)
        Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
    public:
        explicit PhonemeEntity(QObject *parent = nullptr);
        ~PhonemeEntity();

    public:
        enum Type {
            Ahead,
            Normal,
            Final,
        };
        Q_ENUM(Type)

        Type type() const;
        void setType(Type type);

        QString token() const;
        void setToken(const QString &token);

        int start() const;
        void setStart(int start);

    Q_SIGNALS:
        void typeChanged(Type type);
        void tokenChanged(const QString &token);
        void startChanged(int start);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        PhonemeEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    DSPXMODEL_DECLARE_VECTOR_CLASS(PhonemeListEntity, PhonemeEntity)

    class DSPXMODEL_EXPORT PhonemeInfoEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(QJsonArray original READ original WRITE setOriginal NOTIFY originalChanged)
    public:
        explicit PhonemeInfoEntity(QObject *parent = nullptr);
        ~PhonemeInfoEntity();

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        PhonemeListEntity *edited() const;

    Q_SIGNALS:
        void originalChanged(const QJsonArray &org);

    protected:
        PhonemeInfoEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT VibratoInfoEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(double start READ start WRITE setStart NOTIFY startChanged)
        Q_PROPERTY(double end READ end WRITE setEnd NOTIFY endChanged)
        Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
        Q_PROPERTY(double phase READ phase WRITE setPhase NOTIFY phaseChanged)
        Q_PROPERTY(double amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
        Q_PROPERTY(double offset READ offset WRITE setOffset NOTIFY offsetChanged)
    public:
        explicit VibratoInfoEntity(QObject *parent = nullptr);
        ~VibratoInfoEntity();

    public:
        double start() const;
        void setStart(double start);

        double end() const;
        void setEnd(double end);

        double frequency() const;
        void setFrequency(double frequency);

        double phase() const;
        void setPhase(double phase);

        double amplitude() const;
        void setAmplitude(double amplitude);

        double offset() const;
        void setOffset(double offset);

        DoublePointListEntity *points() const;

    Q_SIGNALS:
        void startChanged(double start);
        void endChanged(double end);
        void frequencyChanged(double frequency);
        void phaseChanged(double phase);
        void amplitudeChanged(double amplitude);
        void offsetChanged(double offset);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        VibratoInfoEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    class DSPXMODEL_EXPORT NoteEntity : public Substate::StructEntityBase {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
        Q_PROPERTY(int keyNumber READ keyNumber WRITE setKeyNumber NOTIFY keyNumberChanged)
        Q_PROPERTY(QString lyric READ lyric WRITE setLyric NOTIFY lyricChanged)
        Q_PROPERTY(QString pronunciation READ pronunciation WRITE setPronunciation NOTIFY
                       pronunciationChanged)
    public:
        explicit NoteEntity(QObject *parent = nullptr);
        ~NoteEntity();

    public:
        int position() const;
        void setPosition(int position);

        int length() const;
        void setLength(int length);

        int keyNumber() const;
        void setKeyNumber(int keyNumber);

        QString lyric() const;
        void setLyric(const QString &lyric);

        QString orgPronunciation() const;
        void setOrgPronunciation(const QString &orgPronunciation);

        QString pronunciation() const;
        void setPronunciation(const QString &pronunciation);

        PhonemeInfoEntity *phonemeInfo() const;
        VibratoInfoEntity *vibratoInfo() const;

    Q_SIGNALS:
        void positionChanged(int position);
        void lengthChanged(int length);
        void keyNumberChanged(int keyNumber);
        void lyricChanged(const QString &lyric);
        void orgPronunciationChanged(const QString &orgPronunciation);
        void pronunciationChanged(const QString &pronunciation);

    protected:
        void sendAssigned(int index, const Value &val, const Value &oldVal) override;

        NoteEntity(Substate::Node *node, bool init, QObject *parent = nullptr);
        friend class DspxInitializer;
    };

    DSPXMODEL_DECLARE_SHEET_CLASS(NoteListEntity, NoteEntity)

}

#endif // NOTEENTITY_H
