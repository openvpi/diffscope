#ifndef AUDIOTROUBLESHOOTINGDIALOG_H
#define AUDIOTROUBLESHOOTINGDIALOG_H

#include <QWizard>

namespace Audio::Internal {

    class AudioTroubleshootingDialog : public QWizard {
        Q_OBJECT
    public:
        explicit AudioTroubleshootingDialog(QWidget *parent = nullptr);
        ~AudioTroubleshootingDialog() override;
    };

} // Audio

#endif //AUDIOTROUBLESHOOTINGDIALOG_H
