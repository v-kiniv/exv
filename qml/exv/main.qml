import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: rec
    width: 800
    height: 460
    color: "#fff"

    property string mainTitle: "No Title"
    property string subTitle: "No Title"
    property string coverUrl: ""
    property bool showPlaylist: false




//    MouseArea {
//        anchors.fill: parent
//        onClicked: {

//            //            exv.testSlot()
//            //            Qt.quit();
//            //            videoPlayerItem.playVideo()
//            console.log("Looog")
//        }
//    }


    VideoView {
        enabled: false
        id: videoView
    }

    SearchPanel {
        visible: true
        id: searchPanel
    }





}
