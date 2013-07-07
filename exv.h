#ifndef EXV_H
#define EXV_H

#include <QObject>
#include "qtquick2applicationviewer.h"
#include "exua.h"

class Exv : public QObject
{
    Q_OBJECT

private:
    QtQuick2ApplicationViewer *m_Viewer;
    bool m_bFullScreen;
    Exua *m_exua;

public:
    explicit Exv(QtQuick2ApplicationViewer *viewer, QObject *parent = 0);
    void setViewer(QtQuick2ApplicationViewer* v) { m_Viewer = v; }
    
signals:
    
public slots:
    void testSlot();
    void toggleFullscreen();
    void searchVideo(QString);
    void getPlaylist(QString);
    
};

#endif // EXV_H
