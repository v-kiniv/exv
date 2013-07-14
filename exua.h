#ifndef EXUA_H
#define EXUA_H

#include "parser.h"
#include <QObject>


class Exua : public Parser
{
    Q_OBJECT

private:

public:
    explicit Exua(QObject *parent = 0);

    void searchVideo(QString str);
    void getPlaylist(QString exid);

signals:

private slots:
    void searchReply(QByteArray);
    void playlistReply(QByteArray);

};

#endif // EXUA_H
