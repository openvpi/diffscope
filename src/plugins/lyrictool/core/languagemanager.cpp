#include "languagemanager.h"
#include "languagemanager_p.h"

namespace LyricTool {

    LanguageManagerPrivate::LanguageManagerPrivate() {
    }

    LanguageManagerPrivate::~LanguageManagerPrivate() {
    }

    void LanguageManagerPrivate::init() {
    }

    LanguageManager::LanguageManager(QObject *parent)
        : LanguageManager(*new LanguageManagerPrivate(), parent) {
    }

    LanguageManager::~LanguageManager() {
    }

    LanguageManager::LanguageManager(LanguageManagerPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
