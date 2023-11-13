#include "File.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QJSEngine>

#include "GlobalObject.h"

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
    QVector<char> data(readSize);
    int actualSize = f->read(data.data(), readSize);
    if (actualSize == -1) {
        JS_THROW(f->errorString());
        return {};
    }
    data.resize(actualSize);
    auto jsArrayData = jsGlobal->engine()->toScriptValue(data);
    auto jsUint8ArrayData = jsGlobal->engine()->globalObject().property("Uint8Array").callAsConstructor({jsArrayData});
    return jsUint8ArrayData;
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
    } else if (data.isArray()) {
        QVector<char> arr;
        arr.reserve(data.property("length").toInt());
        for (const auto &v: data.toVariant().toList()) {
            bool ok;
            arr.append(v.toInt(&ok));
            if (!ok) {
                JS_THROW(QJSValue::TypeError, "Invalid data type to write to file");
                return 0;
            }
        }
        actualSize = f->write(arr.data(), arr.size());
    } else if (data.property("BYTES_PER_ELEMENT").toInt() == 1) {
        QVector<char> arr;
        arr.reserve(data.property("length").toInt());
        for (int i = 0; i < data.property("length").toInt(); i++) {
            arr.append(data.property(i).toInt());
        }
        actualSize = f->write(arr.data(), arr.size());
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
