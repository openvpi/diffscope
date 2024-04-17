#ifndef AUDIO_ADDONLOADER_H
#define AUDIO_ADDONLOADER_H

#include <QList>
#include <QMetaObject>
#include <QTimer>

namespace Audio {

    template <class AddonType>
    class AddOnLoader {
    public:
        explicit AddOnLoader(const QList<QMetaObject *> &classList, QObject *context) : m_context(context) {
            for (auto clazz: classList) {
                m_addOns.append(qobject_cast<AddonType *>(clazz->newInstance(Q_ARG(QObject*, context))));
            }
        }
        ~AddOnLoader() = default;

        QList<AddonType *> addOns() const {
            return m_addOns;
        }

        bool initializeAll() {
            return std::all_of(m_addOns.cbegin(), m_addOns.cend(), [](AddonType *addOn) {
                return addOn->initialize();
            });
        }

        void extensionInitializedAll() {
            std::for_each(m_addOns.cbegin(), m_addOns.cend(), [](AddonType *addOn) {
                addOn->extensionInitialized();
            });
        }

        void delayedInitializeAll() {
            auto *delayedInitializeTimer = new QTimer;
            delayedInitializeTimer->setInterval(5);
            delayedInitializeTimer->setSingleShot(true);
            QObject::connect(delayedInitializeTimer, &QTimer::timeout, m_context, [=] {
                while (!m_delayedInitializeQueue.empty()) {
                    auto addOn = m_delayedInitializeQueue.takeFirst();
                    bool delay = addOn->delayedInitialize();
                    if (delay)
                        break;
                }
                if (m_delayedInitializeQueue.empty()) {
                    delete delayedInitializeTimer;
                } else {
                    delayedInitializeTimer->start();
                }
            });
            delayedInitializeTimer->start();
        }

    private:
        QList<AddonType *> m_addOns;
        QList<AddonType *> m_delayedInitializeQueue;
        QObject *m_context;
    };

} // Audio

#endif // AUDIO_ADDONLOADER_H
