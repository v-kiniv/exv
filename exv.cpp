#include "exv.h"

#include <QDebug>

Exv::Exv(QObject *parent) :
    QObject(parent)
{
    m_bFullScreen = false;
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
