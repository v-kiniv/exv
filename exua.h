#ifndef EXUA_H
#define EXUA_H

#include "listmodel.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Exua : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *m_nManager;
    QNetworkRequest m_nRequest;
    QNetworkReply *m_nReply;

    ListModel *m_searchModel;
    ListModel *m_playlistModel;
public:
    explicit Exua(QObject *parent = 0);

    void searchVideo(QString str);
    void getPlaylist(QString exid);

    ListModel* searchModel() { return m_searchModel; }
    ListModel* playlistModel() { return m_playlistModel; }
    
signals:

private slots:

public slots:
    void searchReply(QNetworkReply*);
    void playlistReply(QNetworkReply*);

};

#endif // EXUA_H
