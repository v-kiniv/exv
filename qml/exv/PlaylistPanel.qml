import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: playlistPanel
    visible: showPlaylist
    enabled: showPlaylist
    width: rec.width / 2
    height: parent.height
    color: "#5a5a57"
    opacity: 0.7

    
    Component {
        id: highlightBar
        Rectangle {
            width: playlistPanel.width
            height: 20
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
            height: 20
            x: 5
            width: playlistPanel.width
            Text {
                height: 5
                color: "#b8dffd"
                text: index+1 + ". " +name
                font.pointSize: 12
                renderType: Text.NativeRendering
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
                    console.log("Start playing...")
                    mediaplayer.source = url
                    videoPlayerItem.playVideo()
                    subTitle = name
//                    showPlaylist = false
                    videoout.focus = true
//                    console.log(playlistModel.getItem(1).getUrl());
                    currentFileIndex = index
                    showPlaylist = false
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
        width: rec.width / 2
        height: playlistPanel.height - 50
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
