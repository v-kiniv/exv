import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0



Rectangle {
    id: searchPanel
    color: "#414242"
    anchors.fill: parent

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
                }
                Row {
                    spacing: 20

                    Image {
                        source: image + "?100"
                    }

                    Text {
                        height: 100
                        color: "#f9f4f4"
                        text: desc
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
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: {
                    searchList.currentIndex = index
                }

                onClicked: {
                    console.log("Index"+index+"; Exid: "+exid)

                }

                onDoubleClicked: {
                    console.log("DDDDDDDDd")
//                    searchList.re
                }
            }
        }
    }
    
    ListView {
        id: searchList
        keyNavigationWraps: true
        focus: true
        highlight: highlightBar
        highlightFollowsCurrentItem: false
        width: rec.width
        height: searchPanel.height - 50
//        boundsBehavior: Flickable.StopAtBounds
        topMargin: 50
        
        model: searchModel
        delegate: delegate
        //                anchors.bottom: parent.bottom
        
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
            TextInput {
                id: searchInput
                color: "#f9f8f8"
                text: qsTr("Search...")
                cursorVisible: false
                anchors.leftMargin: 9
                anchors.topMargin: 7
                anchors.fill: parent
                font.pointSize: 14
                horizontalAlignment: TextInput.AlignLeft
                selectionColor: "#8fd1ff"
                font.pixelSize: 14
                onFocusChanged: {
                    if(searchInput.text == "Search...")
                        searchInput.text = ""
                }
                onAccepted: {
                    console.log("SEARCH")
                    
                    exv.searchVideo(searchInput.text)
                    searchList.update()
                    //                            searchList.update()
                }
            }
        }
        
        
        
        
    }
    
    
}
