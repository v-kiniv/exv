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
    property int currentFileIndex: -1
    property string currentId: ""


    VideoView {
        enabled: true
        focus: true
        id: videoView
    }

    SearchPanel {
        visible: false
        focus: false
        id: searchPanel
    }
}
