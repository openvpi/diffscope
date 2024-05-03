#include "builtinformatentry.h"

#include <memory>
#include <algorithm>
#include <set>

#include <QFile>
#include <QDialog>
#include <QBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDoubleValidator> // TODO add expression double validator
#include <QPushButton>

#include <QMWidgets/qmdecoratorv2.h>

#include <SVSCraftWidgets/expressionspinbox.h>

#include <TalcsFormat/AudioFormatIO.h>

namespace Audio {

    class AudioFormatIOObject : public QObject, public talcs::AudioFormatIO {
    public:
        explicit AudioFormatIOObject(QIODevice *stream = nullptr, QObject *parent = nullptr) : QObject(parent), talcs::AudioFormatIO(stream) {

        }
        ~AudioFormatIOObject() override = default;
    };

    BuiltInFormatEntry::BuiltInFormatEntry(QObject *parent) : FormatEntry(parent) {
        std::set<QString> extensionHintSet;
        for (const auto &fmtInfo : talcs::AudioFormatIO::availableFormats()) {
            QStringList fmtExtensions;
            fmtExtensions.append(fmtInfo.extension);
            if (fmtInfo.extension == "raw") {
                for (const auto &subtypeInfo : fmtInfo.subtypes)
                    m_rawSubtypes.append({subtypeInfo.name, subtypeInfo.subtype});
            }
            for (const auto &subtypeInfo : fmtInfo.subtypes) {
                fmtExtensions += subtypeInfo.extensions;
            }
            extensionHintSet.insert(fmtExtensions.cbegin(), fmtExtensions.cend());
            std::transform(fmtExtensions.cbegin(), fmtExtensions.cend(), fmtExtensions.begin(), [](const QString &extension) {
                return "*." + extension;
            });
            m_filters.append(QString("%1 (%2)").arg(fmtInfo.name, fmtExtensions.join(" ")));
        }
        m_extensionHints = QStringList(extensionHintSet.cbegin(), extensionHintSet.cend());
    }

    BuiltInFormatEntry::~BuiltInFormatEntry() = default;

    QStringList BuiltInFormatEntry::filters() const {
        return m_filters;
    }
    QStringList BuiltInFormatEntry::extensionHints() const {
        return m_extensionHints;
    }
    talcs::AbstractAudioFormatIO *BuiltInFormatEntry::getFormatOpen(const QString &filename, QVariant &userData, QWidget *win) {
        std::unique_ptr<AudioFormatIOObject> io = std::make_unique<AudioFormatIOObject>();
        std::unique_ptr<QFile> f = std::make_unique<QFile>(filename, io.get());
        if (!f->open(QIODevice::ReadOnly))
            return nullptr;
        io->setStream(f.release());
        if (filename.endsWith(".raw")) {
            QDialog dlg(win);
            auto mainLayout = new QVBoxLayout;
            auto optionsLayout = new QFormLayout;

            auto subtypeComboBox = new QComboBox;
            for (const auto &[name, subtype] : m_rawSubtypes) {
                subtypeComboBox->addItem(name, subtype);
            }
            optionsLayout->addRow(tr("Option"), subtypeComboBox);

            auto channelCountSpinBox = new SVS::ExpressionSpinBox;
            channelCountSpinBox->setMinimum(1);
            optionsLayout->addRow(tr("Channel"), channelCountSpinBox);

            auto sampleRateComboBox = new QComboBox;
            sampleRateComboBox->addItems({"8000", "11025", "12000", "16000", "22050", "24000",
                                          "32000", "44100", "48000", "64000", "88200", "96000",
                                          "128000", "176400", "192000", "256000", "352800",
                                          "384000"});
            sampleRateComboBox->setEditable(true);
            sampleRateComboBox->setValidator(new QDoubleValidator(0.01, std::numeric_limits<double>::max(), 2));
            optionsLayout->addRow(tr("Sample rate"), sampleRateComboBox);

            auto byteOrderComboBox = new QComboBox;
            byteOrderComboBox->addItem(tr("System"), talcs::AudioFormatIO::SystemOrder);
            byteOrderComboBox->addItem(tr("Little-endian"), talcs::AudioFormatIO::LittleEndian);
            byteOrderComboBox->addItem(tr("Big-endian"), talcs::AudioFormatIO::BigEndian);
            optionsLayout->addRow(tr("Byte order"), byteOrderComboBox);

            mainLayout->addLayout(optionsLayout);

            auto buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch();
            auto okButton = new QPushButton(tr("OK"));
            buttonLayout->addWidget(okButton);
            connect(okButton, &QAbstractButton::clicked, &dlg, &QDialog::accept);
            mainLayout->addLayout(buttonLayout);

            dlg.setLayout(mainLayout);
            dlg.setWindowTitle(tr("Configure Raw Data"));
            dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            if (dlg.exec() != QDialog::Accepted)
                return nullptr;
            userData = QVariantMap({
                {"subtype", subtypeComboBox->currentData()},
                {"channelCount", channelCountSpinBox->value()},
                {"sampleRate", QLocale(qIDec->locale()).toDouble(sampleRateComboBox->currentText())},
                {"byteOrder", byteOrderComboBox->currentData()}
            });
            io->setFormat(talcs::AudioFormatIO::RAW | subtypeComboBox->currentData().toInt() | byteOrderComboBox->currentData().toInt());
            io->setChannelCount(channelCountSpinBox->value());
            io->setSampleRate(QLocale(qIDec->locale()).toDouble(sampleRateComboBox->currentText()));

        }
        if (!io->open(talcs::AbstractAudioFormatIO::Read))
            return nullptr;
        io->close();
        return io.release();
    }
    talcs::AbstractAudioFormatIO *BuiltInFormatEntry::getFormatLoad(const QString &filename, const QVariant &userData) {
        std::unique_ptr<AudioFormatIOObject> io = std::make_unique<AudioFormatIOObject>();
        std::unique_ptr<QFile> f = std::make_unique<QFile>(filename, io.get());
        if (!f->open(QIODevice::ReadOnly))
            return nullptr;
        io->setStream(f.release());
        if (filename.endsWith(".raw")) {
            auto rawOptions = userData.toMap();
            io->setFormat(talcs::AudioFormatIO::RAW | rawOptions.value("subtype").toInt() | rawOptions.value("byteOrder").toInt());
            io->setChannelCount(rawOptions.value("channelCount").toInt());
            io->setSampleRate(rawOptions.value("sampleRate").toDouble());
        }
        if (!io->open(talcs::AbstractAudioFormatIO::Read))
            return nullptr;
        io->close();
        return io.release();
    }
}