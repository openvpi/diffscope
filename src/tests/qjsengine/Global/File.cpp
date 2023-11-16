#include "File.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QJSEngine>

#include "GlobalObject.h"
#include "../ObjectWrapper.h"

File::File(const QString &path) : f(new QFile(path, this)) {
}

File::~File() {
    f->close();
}

QString File::name() const {
    return f->fileName();
}

QJSValue File::lastModifiedDate() const {
    auto date = QFileInfo(*f).lastModified();
    if (!date.isValid())
        return QJSValue::NullValue;
    return jsGlobal->engine()->globalObject().property("Date").callAsConstructor({date.toString(Qt::ISODateWithMs)});
}

int File::size() const {
    return f->size();
}

void File::resize(int newSize) {
    if (!f->resize(newSize))
        JS_THROW(f->errorString());
}

int File::pos() const {
    return f->pos();
}

void File::seek(int pos) {
    if (!f->seek(pos))
        JS_THROW(f->errorString());
}

void File::open(int openMode) {
    if (!f->open(QIODevice::OpenMode(openMode)))
        JS_THROW(f->errorString());
}

int File::openMode() const {
    return f->openMode();
}

void File::close() {
    f->close();
}

QJSValue File::read(int readSize) {
    QByteArray data(readSize, Qt::Uninitialized);
    int actualSize = f->read(data.data(), readSize);
    if (actualSize == -1) {
        JS_THROW(f->errorString());
        return {};
    }
    data.resize(actualSize);
    return ObjectWrapper::toUint8Array(data, jsGlobal->engine()).property("buffer");
}

QJSValue File::readAll() {
    return read(f->size() - f->pos());
}

QString File::readAllAsUtf8() {
    auto s = QString::fromUtf8(f->readAll());
    if (f->error() != QFileDevice::NoError) {
        JS_THROW(f->errorString());
        return {};
    }
    return s;
}

int File::write(const QJSValue &data) {
    int actualSize;
    if (data.isString()) {
        actualSize = f->write(data.toString().toUtf8());
    } else if (jsGlobal->engine()->globalObject().property("ArrayBuffer").property("isView").call({data}).toBool()) {
        return write(data.property("buffer"));
    } else if (data.isArray()) {
        return write(jsGlobal->engine()->globalObject().property("Uint8Array").callAsConstructor({data}).property("buffer"));
    } else if (data.hasProperty("byteLength")) {
        auto uint8Data = jsGlobal->engine()->globalObject().property("Uint8Array").callAsConstructor({data});
        actualSize = f->write(ObjectWrapper::fromUint8Array(uint8Data, jsGlobal->engine()));
    } else {
        JS_THROW(QJSValue::TypeError, "Invalid data type to write to file");
        return 0;
    }
    if (actualSize == -1) {
        JS_THROW(f->errorString());
        return 0;
    }
    return actualSize;
}
void File::flush() {
    if (!f->flush())
        JS_THROW(f->errorString());
}
