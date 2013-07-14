#include "exua.h"
#include "vitem.h"
#include "pitem.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QRegExp>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Exua::Exua(QObject *parent) :
    QObject(parent)
{
    m_nManager = new QNetworkAccessManager();
    m_nRequest.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.121 Safari/535.2");
}

void Exua::searchVideo(QString str)
{
    qDebug() << "Start";
    QUrl url(QString("http://www.ex.ua/search?s=%0").arg(str));

    m_nRequest.setUrl(url);

    m_nManager->disconnect();
    connect(m_nManager, SIGNAL(finished(QNetworkReply*)), SLOT(searchReply(QNetworkReply*)));
    m_nManager->get(m_nRequest);
}

void Exua::getPlaylist(QString exid)
{
    qDebug() << "Start";
    QUrl url(QString("http://www.ex.ua/view/%0").arg(exid));

    m_nRequest.setUrl(url);

    m_nManager->disconnect();
    connect(m_nManager, SIGNAL(finished(QNetworkReply*)), SLOT(playlistReply(QNetworkReply*)));
    m_nManager->get(m_nRequest);
}

void Exua::searchReply(QNetworkReply *reply)
{
    qDebug() << "End";
    QString content = reply->readAll();
//    qDebug() << content;

    QRegExp rxTable("<table width=100% border=0 cellpadding=0 cellspacing=8 class=panel>(.*)</table>");
    rxTable.setMinimal(true);
    rxTable.indexIn(content);
    QString tableContent = rxTable.capturedTexts()[0];

    m_searchModel->clear();


//    QRegExp rxTd("<td>(.*)</td>");
    QRegExp rxTd("<tr><td><a href='/view/([\\d]*)'><img src='(.*)\\?[\\d]+' width='[\\d]+' height='[\\d]+' border='0' align='left' style='.*' alt='.*'></a><a href='/view/[\\d]+'><b>(.*)</b></a><br><a href='/user/.*'>.*</a>.*<p>(.*)<p><a href='/view_comments/\\1' class=info>.*</a><p><small>Файлов: ([\\d]*).*</small></td></tr>");
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
//    qDebug() << m_searchModel->rowCount()

}


void Exua::playlistReply(QNetworkReply *reply)
{
    qDebug() << "End";
    QString content = reply->readAll();

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

//    qDebug() << jdoc.isArray();
//    qDebug() << jdoc.isObject();


}
