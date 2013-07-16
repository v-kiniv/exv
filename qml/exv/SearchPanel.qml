import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0



Rectangle {
    id: searchPanel
    color: "#414242"
    anchors.fill: parent

    function focus2List() {
        searchInput.focus = false
        searchList.focus = true
    }

    function itemSeleced() {
        exv.getPlaylist(searchModel.getItem(searchList.currentIndex).exid())
        mainTitle = searchModel.getItem(searchList.currentIndex).name()
        coverUrl = searchModel.getItem(searchList.currentIndex).image()
        videoView.visible = true
        videoView.enabled = true
        showPlaylist = true
        searchPanel.visible = false
        searchPanel.enabled = false
        videoView.focusOnPlayer()
        exv.setTitle(mainTitle)

    }

    // Search ListView
    Component {
           id: highlightBar
           Rectangle {
               width: searchPanel.width; height: 150
               color: "#080809"
               y: searchList.currentItem.y;
               Behavior on y { SpringAnimation { spring: 2; damping: 0.1; mass: 0.3 } }
           }
       }

    Component {
        id: delegate
        Item {
            id: wrapper
            height: 150
            x: 20
            width: searchPanel.width
            Column {
                spacing: 10
                Text {
                    height: 30
                    color: "#b8dffd"
                    text: name
                    font.pointSize: 16
                    renderType: Text.NativeRendering
                }
                Row {
                    spacing: 20

                    Image {
                        source: image + "?100"
                    }

                    Text {
                        height: 100
                        color: "#f9f4f4"
                        text: desc+"<br><b>"+filesCount+"</b> files"
                        renderType: Text.NativeRendering
                    }
                }

            }
            states: State {
                name: "Current"
                when: wrapper.ListView.isCurrentItem
                PropertyChanges {
                    target: wrapper
                    x: 50
                }
            }
            transitions: Transition {
                NumberAnimation {
                    properties: "x"
                    duration: 200
                }
            }
            Keys.onReturnPressed: {
                searchPanel.itemSeleced()
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: {
                    searchList.currentIndex = index
                }

                onClicked: {
                    searchPanel.itemSeleced()
                }
            }
        }
    }
    
    ListView {
        id: searchList
        keyNavigationWraps: true
        highlight: highlightBar
        highlightFollowsCurrentItem: false
        width: rec.width
        height: searchPanel.height - 50
        topMargin: 50
        model: searchModel
        delegate: delegate
        
        Rectangle {
            id: rectangle1
            height: 30
            color: "#000"
            radius: 2
            border.color: "#6b6a6a"
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            Text {
                id: name
                text: searchInput.text == '' ? 'Search...' : '';
                color: "#B5B5B5"
                anchors.leftMargin: 9
                anchors.topMargin: 7
                anchors.fill: parent
                font.pixelSize: 14
                renderType: Text.NativeRendering
            }
            TextInput {
                id: searchInput
                color: "#f9f8f8"
                Component.onCompleted: searchInput.forceActiveFocus()
                cursorVisible: true
                anchors.leftMargin: 9
                anchors.topMargin: 7
                anchors.fill: parent
                horizontalAlignment: TextInput.AlignLeft
                selectionColor: "#8fd1ff"
                font.pixelSize: 14
                renderType: Text.NativeRendering
                focus: true

                onAccepted: {
                    console.log("SEARCH")
                    
                    exv.searchVideo(searchInput.text)
//                    searchInput.focus = false
                }
            }
        }
    }
}
