#include "exv.h"

#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QFile>
#include <QMediaPlayer>


Exv::Exv(QtQuick2ApplicationViewer *viewer, QObject *parent) :
    QObject(parent), m_Viewer(viewer),
    m_nReply2(0)
{
    m_bFullScreen = false;

    m_searchModel = new ListModel(new VItem, this);
    m_favModel = new ListModel(new VItem, this);
    m_playlistModel = new ListModel(new PItem, this);

    m_exua = new Exua();
    m_Viewer->rootContext()->setContextProperty("searchModel", m_searchModel);
    m_Viewer->rootContext()->setContextProperty("favModel", m_favModel);
    m_Viewer->rootContext()->setContextProperty("playlistModel", m_playlistModel);
    m_exua->setSearchModel(m_searchModel);
    m_exua->setPlaylistModel(m_playlistModel);
//    m_exua->searchVideo("X-files");

    connect(m_exua, SIGNAL(searchComleted()), SLOT(onSearchCompleted()));

    load();

    m_nManager = new QNetworkAccessManager();
    m_nReply = 0;

    QFile *m_iodevice = new QFile("/home/yuberion/e6fbeed5e113ffc69b5ac7b82687d6cf.flv", this);
//    QFile *m_iodevice = new QFile("/home/yuberion/9851ec63834380c6f3a9303dbf596c5b.flv", this);
    if (!m_iodevice->open(QIODevice::ReadOnly))
        qDebug() << "failed to open QIODevice";

    m_player = new QMediaPlayer;

    m_adapter = new VideoAdapter(m_player);
    m_player->setVideoOutput(m_adapter);
    connect(m_adapter, SIGNAL(resetMedia()), SLOT(onResetMedia()));

//    VideoAdapter *adapter = new VideoAdapter(player);
//    adapter->setFormat(vs->surfaceFormat());

//    connect(vs, SIGNAL(frameChanged(QVideoFrame)), adapter, SLOT(onNewVideoContentReceived(QVideoFrame)));

//    player->setMedia(QMediaContent(), m_iodevice);
    m_player->setNotifyInterval(50);

    m_Viewer->rootContext()->setContextProperty("player", m_adapter);


//    setSource("http://n29.file-cdn.com/ff/MjhkNGQ2ZjdhMjIxMWQwNTU0YjQ3YWZjNjBiMTIwMDF8ZnN1YXwzMjU3NTgyODM0fDEwMDAwfDJ8MHxvfDI5fDczMjRmNzViYWZmYTE4NmE5MTM3OGI2ODE0YTZjYzM2fDB8MzA6anww/play_2dbjgqzoxpyou.mp4");
    setSource("http://www.ex.ua/load/6801557");
    m_bSecondReady = false;
//    getEnd(26701222, 364652544);
//    getEnd(337998906, 364652544);
    getEnd(337391688, 337391688+(1024*1024));

//    player->play();

}

void Exv::getEnd(qint64 a, qint64 b)
{
    if(m_nReply2)
        m_nReply2->abort();
    qDebug() << "PART: " << a << " to " << b;
    QNetworkRequest request2(QUrl("http://fs6.www.ex.ua/load/89eb4d4f0ab6c621e24547d286f7515d/6801557/X-Files 1x04 - The Jersey Devil.avi"));
    ////////////
//    m_nPos = 364652544 - (1024*10);
    m_nPos = a;
    m_nStartPos = a;
    m_nEndPos = b;
    QByteArray data;
    QString strData("bytes=" + QString::number(m_nPos) + "-");

    data = strData.toLatin1();
    request2.setRawHeader("Range", data);
    ////////////


    m_nReply2 = m_nManager->get(request2);
    connect(m_nReply2, SIGNAL(readyRead()), SLOT(readyRead2()));
    connect(m_nReply2, SIGNAL(downloadProgress(qint64,qint64)), SLOT(bufferProgress2(qint64,qint64)));
}

void Exv::readyRead2()
{
    int available = m_nReply2->bytesAvailable();
    m_baBuffer->replace(m_nPos, available, m_nReply2->readAll());

//    qDebug() << "END REPLACED: " << m_nPos << " to " << available;

    m_nPos += available;

}

void Exv::bufferProgress2(qint64 current, qint64 total)
{
    if(current >= (m_nEndPos - m_nStartPos)) {
        m_nReply2->abort();
        if(!m_bSecondReady) {
            getEnd(169329392 - (1024*10), 169329392 + (1024*10));
            m_bSecondReady = true;
        } else {
            if(m_bufferReady)
                m_player->setMedia(QMediaContent(), m_buffer);
        }

        //qDebug() << "Setted source";
//        m_player->setMedia(QMediaContent(QUrl("http://fs6.www.ex.ua/load/89eb4d4f0ab6c621e24547d286f7515d/6801557/X-Files 1x04 - The Jersey Devil.avi")), m_buffer);
//        m_player->play();
//        m_bufferReady = true;
    }
    qDebug() << "END LOADED: " << current << " from " << (m_nEndPos - m_nStartPos);
}


void Exv::setSource(QString url)
{
    if(m_nReply != 0) {
        m_nReply->abort();
    }
    m_baBuffer = new QByteArray(364652544, '\1');
    m_buffer = new Buffer(m_baBuffer, this);
    m_buffer->open(Buffer::ReadOnly);
    m_buffer->seek(0);
    m_bufferPosition = 0;
    m_bufferReady = false;
    m_bufferNullFilled = false;


//    m_nRequest.setUrl(QUrl("http://fs136.www.ex.ua/show/9851ec63834380c6f3a9303dbf596c5b/27223518/9851ec63834380c6f3a9303dbf596c5b.flv"));
//    m_nRequest = QNetworkRequest(QUrl(url));
    m_nRequest.setUrl(QUrl(url));
    m_nReply = m_nManager->get(m_nRequest);
    connect(m_nReply, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(m_nReply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(bufferProgress(qint64,qint64)));
    connect(m_nReply, SIGNAL(downloadProgress(qint64,qint64)), m_adapter, SLOT(bufferProgress(qint64,qint64)));
    connect(m_nReply, SIGNAL(finished()), SLOT(finished()));
//    m_nReply->abort();
}

void Exv::readyRead()
{
    if(m_bufferNullFilled) {
    int available = m_nReply->bytesAvailable();
    m_baPart = m_nReply->readAll();
    m_baBuffer->replace(m_bufferPosition, available, m_baPart);

    m_bufferPosition += available;
    }
}

void Exv::bufferProgress(qint64 current, qint64 total)
{
    if(!m_bufferNullFilled) {
        m_baBuffer->fill('\n', total);
        qDebug() << "Buffer: " << total;
        m_bufferNullFilled = true;
    }
//    qDebug() << current << " of " << total;
    if(current > (1091126) && !m_bufferReady) {
        qint64 left = 364652544 - 1091126;
        while(left > 0) {
            m_baBuffer->replace(364652544 - left, m_baPart.size(), m_baPart);
            left -= m_baPart.size();
            qDebug() << "Left: " << left;
        }
//        qDebug() << "Setted source";
//        m_player->setMedia(QMediaContent(QUrl("http://n29.file-cdn.com/ff/MjhkNGQ2ZjdhMjIxMWQwNTU0YjQ3YWZjNjBiMTIwMDF8ZnN1YXwzMjU3NTgyODM0fDEwMDAwfDJ8MHxvfDI5fDczMjRmNzViYWZmYTE4NmE5MTM3OGI2ODE0YTZjYzM2fDB8MzA6anww/play_2dbjgqzoxpyou.mp4")), m_buffer);
//        m_player->play();
        m_player->setMedia(QMediaContent(QUrl("http://fs6.www.ex.ua/load/89eb4d4f0ab6c621e24547d286f7515d/6801557/X-Files 1x04 - The Jersey Devil.avi")), m_buffer);
//        m_player->set

        m_bufferReady = true;
    }
}

void Exv::finished()
{
    QUrl redirectUrl = m_nReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirectUrl.isEmpty()) {
        setSource(redirectUrl.toString());
        qDebug() << "Redirect to: " << redirectUrl.toString();
    }
}

void Exv::onResetMedia()
{
    qDebug() << "MEDIA RESET";
    m_buffer = new Buffer(m_baBuffer, this);
    m_buffer->open(Buffer::ReadOnly);
    m_buffer->seek(0);
//    m_player->setMedia(QMediaContent());
    m_player->setMedia(QMediaContent(), m_buffer);
}

bool Exv::addToFav(QString id)
{
    if(m_favModel->find(id) == 0) {
        m_favModel->appendRow(static_cast<VItem*>(m_searchModel->find(id))->copy());
        return true;
    } else {
        return false;
    }
}

void Exv::delFromFav(QString id)
{
    if(m_favModel->find(id) != 0) {
        m_favModel->removeRow(m_favModel->indexFromItem(m_favModel->find(id)).row());
    }
}

void Exv::quit()
{
    save();
    m_Viewer->close();
}

void Exv::load()
{
    m_sAppPath = QDir::homePath() + "/.config/exv";
    if(!QDir(m_sAppPath).exists()) {
        QDir().mkdir(m_sAppPath);
    }

    QFile searchFile(m_sAppPath+"/search.dat");
    if(searchFile.exists()) {
        searchFile.open(QFile::ReadOnly);
        QDataStream out(&searchFile);
        int size;
        out >> size;
        for (int i = 0; i < size; ++i) {
            VItem *item = new VItem(m_searchModel);
            item->read(out);
            m_searchModel->appendRow(item);
        }

        out >> size;
        for (int i = 0; i < size; ++i) {
            VItem *item = new VItem(m_favModel);
            item->read(out);
            m_favModel->appendRow(item);
        }

        searchFile.close();
    }
}

void Exv::save()
{
    QFile searchFile(m_sAppPath+"/search.dat");
    searchFile.open(QFile::WriteOnly);
    QDataStream in(&searchFile);

    int size = m_searchModel->rowCount();
    in << size;
    for (int i = 0; i < size; ++i) {
         VItem *item = (VItem*)m_searchModel->getItem(i);
         item->write(in);
    }

    size = m_favModel->rowCount();
    in << size;
    for (int i = 0; i < size; ++i) {
         VItem *item = (VItem*)m_favModel->getItem(i);
         item->write(in);
    }
    searchFile.close();
}

void Exv::onSearchCompleted()
{
    qDebug() << "EXV search completed";
//    m_favModel->appendRow(static_cast<VItem*>(m_searchModel->getItem(0))->copy());
//    Parser *p = static_cast<Parser*>(sender());
}

void Exv::toggleFullscreen()
{
    if(m_bFullScreen)
        m_Viewer->showExpanded();
    else
        m_Viewer->showFullScreen();

    m_bFullScreen = !m_bFullScreen;
}


void Exv::searchVideo(QString str) {
    m_exua->searchVideo(str);
//    m_exua->searchModel()->clear();
}

void Exv::getPlaylist(QString exid) {
    m_exua->getPlaylist(exid);
}

Exv::~Exv()
{
    save();
}
