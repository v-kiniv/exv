#include "buffer.h"
#include <QDebug>

Buffer::Buffer(QByteArray *&ba, QObject *parent) :
    QBuffer(ba, parent)
{

}

bool Buffer::seek(qint64 off)
{
    qDebug() << "Buffer seek to: " << off;
    QBuffer::seek(off);
}
