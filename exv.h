#ifndef EXV_H
#define EXV_H

#include <QObject>
#include "qtquick2applicationviewer.h"

class Exv : public QObject
{
    Q_OBJECT

private:
    QtQuick2ApplicationViewer *m_Viewer;
    bool m_bFullScreen;
public:
    explicit Exv(QObject *parent = 0);
    void setViewer(QtQuick2ApplicationViewer* v) { m_Viewer = v; }
    
signals:
    
public slots:
    void testSlot();
    void toggleFullscreen();
    
};

#endif // EXV_H
