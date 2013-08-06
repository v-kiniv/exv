#ifndef EXV_H
#define EXV_H

#include "qtquick2applicationviewer.h"
#include "exua.h"
#include "videoadapter.h"
#include "buffer.h"

#include <QObject>
#include <QMediaPlayer>
#include <QBuffer>


class Exv : public QObject
{
    Q_OBJECT

private:
    QtQuick2ApplicationViewer *m_Viewer;
    bool m_bFullScreen;
    Exua *m_exua;
    QString m_sAppPath;
    ListModel *m_searchModel;
    ListModel *m_favModel;
    ListModel *m_playlistModel;
    QMediaPlayer *m_player;
    VideoAdapter *m_adapter;

    QNetworkAccessManager *m_nManager;
    QNetworkRequest m_nRequest;
    QNetworkReply *m_nReply;
    QNetworkReply *m_nReply2;
    Buffer *m_buffer;
    QByteArray *m_baBuffer;
    QByteArray m_baPart;
    qint64 m_bufferPosition;
    qint64 m_nPos;
    qint64 m_nStartPos;
    qint64 m_nEndPos;
    bool m_bufferReady;
    bool m_bufferNullFilled;
    bool m_bSecondReady;

    void load();
    void save();
    void getEnd(qint64 a, qint64 b);

public:
    explicit Exv(QtQuick2ApplicationViewer *viewer, QObject *parent = 0);
    ~Exv();
    void setViewer(QtQuick2ApplicationViewer* v) { m_Viewer = v; }

    Q_INVOKABLE bool addToFav(QString id);
    Q_INVOKABLE void delFromFav(QString id);
    Q_INVOKABLE void quit();
    
signals:
    
private slots:
    void onSearchCompleted();
    void readyRead();
    void readyRead2();
    void bufferProgress(qint64 current, qint64 total);
    void bufferProgress2(qint64 current, qint64 total);
    void finished();
    void onResetMedia();

public slots:
    void toggleFullscreen();
    void searchVideo(QString);
    void getPlaylist(QString);
    void setTitle(QString str) { m_Viewer->setTitle(str); }
    void setSource(QString url);
    
};

#endif // EXV_H
