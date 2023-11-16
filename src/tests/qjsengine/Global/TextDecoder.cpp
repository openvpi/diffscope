#include "TextDecoder.h"

#include <QJSEngine>
#include <QTextCodec>

#include "GlobalObject.h"
#include "../ObjectWrapper.h"

TextDecoder::TextDecoder() : TextDecoder("utf-8", {}) {
}

TextDecoder::TextDecoder(const QString &ufLabel) : TextDecoder(ufLabel, {}) {
}

TextDecoder::TextDecoder(const QString &ufLabel, const QJSValue &options)
    : m_codec(QTextCodec::codecForName(ufLabel.toUtf8())), m_fatal(options.property("fatal").toBool()),
      m_ignoreBOM(options.property("ignoreBOM").toBool()) {
    if (!m_codec) {
        JS_THROW(QJSValue::RangeError, "Encoding not supported: " + ufLabel);
    }
}

QString TextDecoder::encoding() const {
    return m_codec->name();
}

bool TextDecoder::fatal() const {
    return m_fatal;
}

bool TextDecoder::ignoreBOM() const {
    return m_ignoreBOM;
}

QString TextDecoder::decode() {
    return decode({}, {});
}

QString TextDecoder::decode(const QJSValue &buffer) {
    return decode(buffer, {});
}

QString TextDecoder::decode(const QJSValue &buffer, const QJSValue &options) {
    auto data = ObjectWrapper::fromUint8Array(buffer, jsGlobal->engine());
    if (state.isNull())
        state.reset(new QTextCodec::ConverterState(m_ignoreBOM ? QTextCodec::IgnoreHeader : QTextCodec::DefaultConversion));
    auto s = m_codec->toUnicode(data, data.size(), state.data());
    if (state->invalidChars && m_fatal) {
        JS_THROW(QJSValue::TypeError, QString("Malformed data encountered when decoding ") + m_codec->name());
        return {};
    }
    if (!options.property("stream").toBool())
        state.reset();
    return s;
}
