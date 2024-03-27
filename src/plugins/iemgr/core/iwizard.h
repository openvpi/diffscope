#ifndef IWIZARD_H
#define IWIZARD_H

#include <QDateTime>
#include <QObject>

#include <QMCore/qmdisplaystring.h>

#include <iemgr/iemgrglobal.h>

namespace IEMgr {

    class IWizardPrivate;

    class IEMGR_EXPORT IWizard : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IWizard)
    public:
        explicit IWizard(const QString &id, QObject *parent = nullptr);
        ~IWizard();

        enum Feature {
            ImportProject = 1,
            ExportProject = 2,
            ImportAudio = 4,
            ExportAudio = 8,
        };
        Q_ENUM(Feature)
        Q_DECLARE_FLAGS(Features, Feature)

        virtual Features features() const = 0;
        virtual QString filter(Feature feature) const;
        virtual bool runWizard(Feature feature, const QString &path, QWidget *parent) = 0;

    public:
        QString id() const;

        QString iconPath() const;
        void setIconPath(const QString &path);

        QMDisplayString description() const;
        void setDescription(const QMDisplayString &description);

        QMDisplayString displayName() const;
        void setDisplayName(const QMDisplayString &displayName);

        QString category() const;
        void setCategory(const QString &category);

        QMDisplayString displayCategory() const;
        void setDisplayCategory(const QMDisplayString &displayCategory);

    protected:
        IWizard(IWizardPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IWizardPrivate> d_ptr;

        friend class IManager;
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS(IEMgr::IWizard::Features)

#endif // IWIZARD_H
