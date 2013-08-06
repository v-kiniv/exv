#include "exua.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QRegExp>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Exua::Exua(QObject *parent) :
    Parser(parent)
{

}

void Exua::searchVideo(QString str)
{
    qDebug() << "Start search request.";
    QUrl url(QString("http://www.ex.ua/search?s=%0").arg(str));
    connect(this, SIGNAL(requestCompleted(QByteArray)), SLOT(searchReply(QByteArray)));
    request(url);
}

void Exua::getPlaylist(QString exid)
{
    qDebug() << "Start playlist request.";
    QUrl url(QString("http://www.ex.ua/view/%0").arg(exid));
    connect(this, SIGNAL(requestCompleted(QByteArray)), SLOT(playlistReply(QByteArray)));
    request(url);
}

void Exua::searchReply(QByteArray data)
{
    qDebug() << "Search request completed.";
    QString content = data;

    QRegExp rxTable("<table width=100% border=0 cellpadding=0 cellspacing=8 class=panel>(.*)</table>");
    rxTable.setMinimal(true);
    rxTable.indexIn(content);
    QString tableContent = rxTable.capturedTexts()[0];

    m_searchModel->clear();

    QRegExp rxTd("<tr><td><a href='/([\\d]*)'><img src='(.*)\\?[\\d]+' width='[\\d]+' height='[\\d]+' border='0' align='left' style='.*' alt='.*'></a><a href='/[\\d]+'><b>(.*)</b></a><br><a href='/user/.*'>.*</a>.*<p>(.*)<p><small>Файлов: ([\\d]*).*</small></td></tr>");
    rxTd.setMinimal(true);


    int pos = 0;
    while (pos >= 0) {
        pos = rxTd.indexIn(tableContent, pos);
        if (pos >= 0) {
            QString exid = rxTd.capturedTexts()[1];
            QString image = rxTd.capturedTexts()[2];
            QString name = rxTd.capturedTexts()[3];
            QString desc = rxTd.capturedTexts()[4];
            QString filesCount = rxTd.capturedTexts()[5];
            if(exid != "")
                m_searchModel->appendRow(new VItem(name, desc, filesCount, exid, image));
            pos += rxTd.matchedLength();
        }
    }
    emit searchComleted();
    disconnect(this, SIGNAL(requestCompleted(QByteArray)), this, SLOT(searchReply(QByteArray)));
}


void Exua::playlistReply(QByteArray data)
{
    qDebug() << "Playlist request completed.";
    QString content = data;

    m_playlistModel->clear();
//    qDebug() << content;

    QRegExp rxInfo("var player_info = new Array\\((.*)\\);.*var player_list = '(.*)';");
    rxInfo.setMinimal(true);
    rxInfo.indexIn(content);
    QString info = "["+rxInfo.capturedTexts()[1]+"]";
    QString list = "["+rxInfo.capturedTexts()[2]+"]";
    info.replace("{ pos", "{ \"pos\"");
    info.replace(", title", ", \"title\"");
    info.replace("'", "\"");
    info.replace(QRegExp(": ([\\d]+),"), ": \"\\1\",");

    QJsonArray infoArray = QJsonDocument::fromJson(info.toUtf8()).array();
    QJsonArray listArray = QJsonDocument::fromJson(list.toUtf8()).array();

    int i = 0;
    foreach (QJsonValue var, infoArray) {
        QString name = var.toObject().take("title").toString();
        QString url = listArray.at(i).toObject().take("url").toString();
        m_playlistModel->appendRow(new PItem(name, url));
        i++;
    }
    emit playlistReady();
    disconnect(this, SIGNAL(requestCompleted(QByteArray)), this, SLOT(playlistReply(QByteArray)));
}
