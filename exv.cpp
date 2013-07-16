#include "exv.h"

#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QFile>

Exv::Exv(QtQuick2ApplicationViewer *viewer, QObject *parent) :
    QObject(parent), m_Viewer(viewer)
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
