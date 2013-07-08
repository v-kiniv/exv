import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Item {
    id: button
    property string imageSrc: "icons/play.png"
    property bool hover: false
    signal clicked()
    
    width: 32
    height: 32
    Image {
        id: image1
        source: button.imageSrc
        
        LevelAdjust {
            anchors.fill: image1
            source: image1
            minimumOutput: "#000fffff"
            
            opacity: button.hover ? 1 : 0.1
            
            Behavior on opacity {
                NumberAnimation{}
            }
        }
    }
    
    MouseArea{
        id: buttonMousearea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            button.hover = true
        }
        onExited: {
            button.hover = false
        }
    }
    
    Component.onCompleted: {
        buttonMousearea.clicked.connect(clicked)
    }
}
