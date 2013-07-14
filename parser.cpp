#include "parser.h"

Parser::Parser(QObject *parent) :
    QObject(parent)
{
    m_nManager = new QNetworkAccessManager();
    m_nRequest.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.121 Safari/535.2");
    connect(m_nManager, SIGNAL(finished(QNetworkReply*)), SLOT(requestReply(QNetworkReply*)));

//    m_searchModel = new ListModel(new VItem, this);
//    m_playlistModel = new ListModel(new PItem, this);
}


void Parser::request(QUrl &url)
{
    m_nRequest.setUrl(url);
    m_nManager->get(m_nRequest);
}

void Parser::requestReply(QNetworkReply *reply)
{
    emit requestCompleted(reply->readAll());
}
