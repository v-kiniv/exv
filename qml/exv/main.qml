import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: rec
    width: 500
    height: 360
    color: "#fff"

    MouseArea {
        anchors.fill: parent
        onClicked: {

            //            exv.testSlot()
            //            Qt.quit();
            //            videoPlayerItem.playVideo()
            console.log("Looog")
        }
    }


    VideoView {
        enabled: false
        id: videoView
    }

    SearchPanel {
        id: searchPanel
    }
}
