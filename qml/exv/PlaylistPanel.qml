import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: playlistPanel
//    visible: showPlaylist
    enabled: showPlaylist
    width: rec.width / 2
    x: -rec.width
    height: parent.height+1
    color: "#5a5a57"
    opacity: 0.7
    y: -1
    border {
        width: 1
        color: "#5E7781"
    }

    function listUp () {
        console.log("list up")
        playlist.decrementCurrentIndex()
    }

    function listDown () {
        console.log("list down")
        playlist.incrementCurrentIndex()
    }

    function listSelect () {
        console.log("list select")
        currentFileIndex = playlist.currentIndex
        videoPlayerItem.sourceChanged()
        videoout.focus = true
        showPlaylist = false
    }

    NumberAnimation on x {
        easing.type: Easing.InCurve
        running: !showPlaylist
        duration: 300
        from: 0
        to: -rec.width
    }

    NumberAnimation on x {
        easing.type: Easing.OutCubic
        running: showPlaylist
        duration: 300
        from: -playlistPanel.width-1
        to: 0
    }

    
    Component {
        id: highlightBar
        Rectangle {
            width: playlistPanel.width
            height: playlist.currentItem.height
            color: "#080809"
            y: playlist.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 2
                    damping: 0.1
                    mass: 0.3
                }
            }
        }
    }
    
    Component {
        id: delegate
        Item {
            id: wrapper
            height: listItem.height * 2
            x: 5
            width: playlistPanel.width
            Text {
                id: listItem
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - 10
                color: "#b8dffd"
                text: index+1 + ". " +name
                font.pointSize: 12
                renderType: Text.NativeRendering
                wrapMode: Text.Wrap


            }
            
            states: State {
                name: "Current"
                when: wrapper.ListView.isCurrentItem
                PropertyChanges {
                    target: wrapper
                    x: 10
                }
            }
            transitions: Transition {
                NumberAnimation {
                    properties: "x"
                    duration: 200
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: {
                    playlist.currentIndex = index
                }
                
                onClicked: {
                    playlistPanel.listSelect()
                }
                
                onDoubleClicked: {
                    
                    //                    playlist.re
                }
            }
        }
    }
    
    ListView {
        id: playlist
        keyNavigationWraps: true
        focus: true
        highlight: highlightBar
        highlightFollowsCurrentItem: false
        width: playlistPanel.width
        height: playlistPanel.height - 80
//        snapMode: ListView.SnapOneItem
        //        boundsBehavior: Flickable.StopAtBounds
        topMargin: 10
        
//        model: ListModel {
//            ListElement { name: "The X-Files - S03 E10 - 731.avi"; url: "/home/yuberion/Изображения/Camera_2013_05/2013-04-10 19.02.02.mp4" }
//            ListElement { name: "The X-Files - S03 E11 - 731.avi"; url: "/home/yuberion/Изображения/Camera_2013_05/2013-03-23 19.35.59.mp4" }
//            ListElement { name: "The X-Files - S03 E12 - 731.avi"; url: "http://www.ex.ua/show/27219842/e4e867f7071be96c30fcdec66c5b0388.flv" }
//            ListElement { name: "The X-Files - S03 E13 - 731.avi"; url: "/home/yuberion/e6fbeed5e113ffc69b5ac7b82687d6cf.flv" }
//        }
        model: playlistModel
        
        delegate: delegate
    }
}
