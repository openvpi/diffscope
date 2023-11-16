#include "TextEncoder.h"

#include <QJSEngine>

#include "../ObjectWrapper.h"
#include "GlobalObject.h"

TextEncoder::TextEncoder() {
}

QJSValue TextEncoder::encode(const QString &string) {
    return ObjectWrapper::toUint8Array(string.toUtf8(), jsGlobal->engine());
}
