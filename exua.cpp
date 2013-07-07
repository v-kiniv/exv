#include "exua.h"
#include "vitem.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QRegExp>
#include <QStringList>

Exua::Exua(QObject *parent) :
    QObject(parent)
{
    m_nManager = new QNetworkAccessManager();
    m_nRequest.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.121 Safari/535.2");

    m_searchModel = new ListModel(new VItem, this);

     qDebug() << m_searchModel->roleNames();
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

ListModel* Exua::searchModel()
{
    return m_searchModel;
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
            int filesCount = rxTd.capturedTexts()[5].toInt();
            if(exid != "")
                m_searchModel->appendRow(new VItem(name, desc, filesCount, exid, image));

//            VItem *item = new VItem(name, desc, filesCount, exid, image);
//            qDebug() << name;
            pos += rxTd.matchedLength();
        }
    }
//    qDebug() << m_searchModel->rowCount()

}
