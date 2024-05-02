#ifndef DSPXDOCUMENT_H
#define DSPXDOCUMENT_H

#include <CoreApi/idocument.h>

#include <dspxmodel/modelentity.h>

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

        class CORE_EXPORT DataModel {
        public:
            enum StateFlag {
                TransactionFlag = 1,
                UndoRedoFlag = 2,
                UndoFlag = 4,
                RedoFlag = 8,
            };

            enum State {
                Idle = 0,
                Transaction = TransactionFlag,
                Undo = UndoFlag | UndoRedoFlag,
                Redo = RedoFlag | UndoRedoFlag,
            };

            inline DataModel() : d(nullptr) {
            }

            inline bool isNull() const {
                return d != nullptr;
            }

            State state() const;
            inline bool inTransaction() const {
                return state() == Transaction;
            }
            inline bool stepChanging() const {
                return state() & UndoRedoFlag;
            }

            QVariantHash stepMessage(int index) const;

            int minimumStep() const;
            int maximumStep() const;
            int currentStep() const;

            void beginTransaction();
            void abortTransaction();
            void commitTransaction(const QVariantHash &message);

            void undo();
            void redo();

            inline bool canUndo() const {
                return currentStep() > minimumStep();
            }
            inline bool canRedo() const {
                return currentStep() < maximumStep();
            }

            QDspx::ModelEntity *model() const;

        protected:
            const DspxDocumentPrivate *d;

            friend class DspxDocument;
        };

        DataModel dataModel() const;

    public:
        QString dataDirectory() const;
        void setDataDirectory(const QString &dir);

        bool accepted() const;
        void setAccepted(bool accepted);

    public:
        bool open(const QString &fileName) override;
        bool openRawData(const QString &suggestFileName, const QByteArray &data);
        bool makeNew(const QString &suggestFileName = {});
        bool recover(const QString &fileName);

        bool save(const QString &fileName) override;
        void changeSavedState(bool saved);

        QByteArray rawData() const;
        bool isOpened() const;
        bool isFile() const;

        ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
        bool reload(ReloadFlag flag, ChangeType type) override;

        QString defaultPath() const override;
        QString suggestedFileName() const override;
        bool isModified() const override;

        void close() override;

    protected:
        friend class DspxSpec;
    };

}

#endif // DSPXDOCUMENT_H
