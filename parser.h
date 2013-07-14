#ifndef PARSER_H
#define PARSER_H

#include "listmodel.h"

#include "vitem.h"
#include "pitem.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Parser : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_nManager;
    QNetworkRequest m_nRequest;
    QNetworkReply *m_nReply;

protected:
    ListModel *m_searchModel;
    ListModel *m_playlistModel;

    void request(QUrl &url);

public:
    explicit Parser(QObject *parent = 0);

    virtual void searchVideo(QString str) = 0;
    virtual void getPlaylist(QString exid) = 0;

    ListModel* searchModel() { return m_searchModel; }
    ListModel* playlistModel() { return m_playlistModel; }
    void setSearchModel(ListModel *model) { m_searchModel = model; }
    void setPlaylistModel(ListModel *model) { m_playlistModel = model; }
    
signals:
    void searchComleted();
    void playlistReady();
    void requestCompleted(QByteArray);

private slots:
    void requestReply(QNetworkReply*);
    
};

#endif // PARSER_H
