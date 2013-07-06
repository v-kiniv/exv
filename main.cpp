#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"

#include "exv.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Exv exv;
    QtQuick2ApplicationViewer viewer;
    exv.setViewer(&viewer);
    viewer.rootContext()->setContextProperty("exv", &exv);
    viewer.setMainQmlFile(QStringLiteral("qml/exv/main.qml"));
    viewer.showExpanded();
//    viewer.showFullScreen();



    return app.exec();
}
