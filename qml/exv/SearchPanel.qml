import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0



Rectangle {
    id: searchPanel
    color: "#414242"
    anchors.fill: parent

    property bool showSearch: true

    function focus2List() {
        searchInput.focus = false
        searchList.focus = true
    }

    function itemSeleced() {
        exv.getPlaylist(searchList.model.getItem(searchList.currentIndex).exid())
        mainTitle = searchList.model.getItem(searchList.currentIndex).name()
        coverUrl = searchList.model.getItem(searchList.currentIndex).image()
        videoView.visible = true
        videoView.enabled = true
        showPlaylist = true
        searchPanel.visible = false
        searchPanel.enabled = false
        videoView.focusOnPlayer()
        exv.setTitle(mainTitle)
        currentId = searchList.model.getItem(searchList.currentIndex).exid()
        currentFileIndex: -1

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

    Column {
    Row {
        spacing: 2
        Rectangle {
            height: 40
            width: searchPanel.width / 2
            color: "black"
            z: 1
            opacity: !searchPanel.showSearch ? 0.5 : 1
            Row {
                anchors.centerIn: parent
                Image {
                    source: "icons/search.png"
                }

                Text {
                    text: "Search"
                    color: "#fff"
                    font.pixelSize: 24
                    renderType: Text.NativeRendering
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: searchPanel.showSearch = true
            }
        }

        Rectangle {
            height: 40
            width: searchPanel.width / 2
            color: "black"
            z: 1
            opacity: searchPanel.showSearch ? 0.5 : 1
            Row {
                anchors.centerIn: parent
                Image {
                    source: "icons/bookmarks.png"
                }

                Text {
                    text: "Bookmarks"
                    color: "#fff"
                    font.pixelSize: 24
                    renderType: Text.NativeRendering
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: searchPanel.showSearch = false
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
        topMargin: searchPanel.showSearch ? 50 : 10
        model: searchPanel.showSearch ? searchModel : favModel
        delegate: delegate
        z: -1

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
            visible: searchPanel.showSearch
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
}
