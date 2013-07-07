#include "exv.h"

#include <QDebug>
#include <QQmlContext>

Exv::Exv(QtQuick2ApplicationViewer *viewer, QObject *parent) :
    QObject(parent), m_Viewer(viewer)
{
    m_bFullScreen = false;

    m_exua = new Exua();
    m_Viewer->rootContext()->setContextProperty("searchModel", m_exua->searchModel());
    m_Viewer->rootContext()->setContextProperty("playlistModel", m_exua->playlistModel());
//    m_exua->searchVideo("X-files");
//    m_exua->getPlaylist("15228917");

}


void Exv::testSlot()
{
    qDebug() << "Zalupa";
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
