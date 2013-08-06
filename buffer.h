#ifndef BUFFER_H
#define BUFFER_H

#include <QBuffer>

class Buffer : public QBuffer
{
    Q_OBJECT
public:
    Buffer(QByteArray*& ba, QObject *parent = 0);

    bool isSequential() { return false; }

    bool seek(qint64 off);

signals:

public slots:

};

#endif // BUFFER_H
