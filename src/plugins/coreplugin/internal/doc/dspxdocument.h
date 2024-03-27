#ifndef DSPXDOCUMENT_H
#define DSPXDOCUMENT_H

#include <CoreApi/idocument.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class DspxSpec;

    class DspxDocumentPrivate;

    class CORE_EXPORT DspxDocument : public IDocument {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxDocument)
    public:
        explicit DspxDocument(QObject *parent = nullptr);
        ~DspxDocument();

    public:
        // Initializers, must call one of them before accessing the instance
        bool open(const QString &fileName) override;
        bool openRawData(const QString &suggestFileName, const QByteArray &data);
        void makeNew(const QString &suggestFileName = {});
        bool recover(const QString &fileName);

        // Save functions
        bool save(const QString &fileName) override;
        bool saveRawData(QByteArray *data) const;

        bool isOpened() const;

        ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
        bool reload(ReloadFlag flag, ChangeType type) override;

        QString defaultPath() const override;
        QString suggestedFileName() const override;
        bool isModified() const override;

        void close() override;

    protected:
        DspxDocument(DspxDocumentPrivate &d, QObject *parent = nullptr);

        friend class DspxSpec;
    };

}

#endif // DSPXDOCUMENT_H
