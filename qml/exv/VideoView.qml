import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: videoView
    anchors.fill: parent
    color: "#000"

    Item {
        //        visible: false;
        id: videoPlayerItem
        anchors.fill: parent
        property bool isVideoPlaying: mediaplayer.hasVideo // property to know if videoPlaying id ongoing or not
        /*
                                                                                                       Functions Which starts Video Playing
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   */
        function playVideo() {
//            mediaplayer.volume = 1.0
            mediaplayer.play()
        }
        /*
                                                                                                       Function Which Stops Video Playing
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   */
        function stopVideo() {
            mediaplayer.stop()
        }
        /*
                                                                                                       Function Which Stops Video Playing
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   */
        function pauseVideo() {
            mediaplayer.pause()
        }

        function playPauseVideo() {

//                        mediaplayer.volume = 1.0
            if (mediaplayer.playbackState == MediaPlayer.PlayingState) {
                mediaplayer.pause()
                //                panel.show = true
            } else {
                mediaplayer.play()
                //                panel.show = false
            }
        }

        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                //                console.log("Button clicked: " + mediaplayer.metaData.resolution.width)
                //                console.log("Button clicked: " + videoout.width)
                videoPlayerItem.playPauseVideo()
            }
            onDoubleClicked: {
                console.log("Full")
                exv.toggleFullscreen()

                //                mediaplayer.play()
                panel.show = false
            }

            onPositionChanged: {
                if (!panel.show)
                    console.log("panel:" + panel.show)
                if (!panel.show)
                    panel.show = true
                if (panelTimer.running)
                    panelTimer.stop()

                if(mediaplayer.playbackState == MediaPlayer.PlayingState)
                    panelTimer.start()
            }

                        cursorShape: (panel.show || playlistPanel.show) ? Qt.ArrowCursor : Qt.BlankCursor
        }

        /*
                                                                                                       Actual QML based Video Component
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   */
        MediaPlayer {
            id: mediaplayer
            //                        source: "/home/yuberion/Изображения/Camera_2013_05/2013-04-10 19.02.02.mp4"
            //            source: "/home/yuberion/Изображения/Camera_2013_05/2013-03-23 19.35.59.mp4"
            //            source: "http://www.ex.ua/show/1174748/e6fbeed5e113ffc69b5ac7b82687d6cf.flv"
//            source: "/home/yuberion/e6fbeed5e113ffc69b5ac7b82687d6cf.flv"
            volume: 1.0

            //            source: "http://www.ex.ua/get/27196651"
            onPositionChanged: {
                //               if (mediaplayer.position > 1000) {
                //                   seekBar.position = mediaplayer.position;
                //               }
                seekBar.position = mediaplayer.position
            }

            onStopped: {
                mediaplayer.source = playlistModel.getItem(++currentFileIndex).getUrl();
                subTitle = playlistModel.getItem(currentFileIndex).getName();
                mediaplayer.play()
            }
        }
        VideoOutput {
            id: videoout
            anchors.fill: parent
            source: mediaplayer
            fillMode: VideoOutput.PreserveAspectFit
            focus: true
            Keys.onSpacePressed: mediaplayer.playbackState
                                 == MediaPlayer.PlayingState ? mediaplayer.pause(
                                                                   ) : mediaplayer.play()
            Keys.onLeftPressed: mediaplayer.seek(mediaplayer.position - 5000)
            Keys.onRightPressed: mediaplayer.seek(mediaplayer.position + 5000)

            Keys.onDigit1Pressed: {
                videoout.fillMode = VideoOutput.PreserveAspectFit
            }
            Keys.onDigit2Pressed: {
                videoout.fillMode = VideoOutput.PreserveAspectCrop
            }
            Keys.onDigit3Pressed: {
                videoout.fillMode = VideoOutput.Stretch
            }

            Keys.onTabPressed: {
                showPlaylist = !showPlaylist
            }

            Keys.onUpPressed: {
                console.log("Volume: "+mediaplayer.volume)
                if(mediaplayer.volume < 1)
                    mediaplayer.volume += 0.1
            }

            Keys.onDownPressed: {
                console.log("Volume: "+mediaplayer.volume)
                if(mediaplayer.volume > 0)
                    mediaplayer.volume -= 0.1
            }

            Keys.onEscapePressed: {
                mediaplayer.pause()
                videoView.visible = false
                videoView.enabled = false
                showPlaylist = false
                searchPanel.visible = true
                searchPanel.enabled = true
                searchPanel.focus2List()
            }
        }
    }

//    FastBlur {
//        anchors.fill: parent
//        source: videoout
//        radius: 64
//        transparentBorder: false
//        visible: mediaplayer.playbackState == MediaPlayer.PausedState
//    }

       BrightnessContrast {
           anchors.fill: parent
           source: videoout
           brightness: -0.7
           contrast: 0
           visible: mediaplayer.playbackState == MediaPlayer.PausedState
       }

    Item {
        id: coverTitle
        width: 500
        height: rec.height - 200
//        color: transparent
        anchors.centerIn: parent
        visible: mediaplayer.playbackState == MediaPlayer.PausedState || mediaplayer.playbackState == MediaPlayer.StoppedState

        Column {
        Image {
            id: coverImage
            height: coverTitle.height - 100
            fillMode: Image.PreserveAspectFit
            width: 500
            source: coverUrl
        }
        Text {
            id: firstTitle
            width: 500
            color: "#fff"
            style: Text.Raised; styleColor: "#000"
            text: mainTitle
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 26
            renderType: Text.NativeRendering
        }
        Text {
            id: secondTitle
            width: 500
            color: "#fff"
            style: Text.Raised; styleColor: "#000"
            text: subTitle
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
            renderType: Text.NativeRendering
        }

        }
    }

    PlaylistPanel {
        id: playlistPanel
    }

    Rectangle {
        id: panel
        height: 40
        width: rec.width
        opacity: 0.7

        property bool show: true
        property int expandedY: parent.height - 40
        property int hiddenY: parent.height - 1

        NumberAnimation on y {
            easing.type: Easing.OutCirc
            running: !panel.show
            duration: 500
            from: panel.expandedY
            to: panel.hiddenY
        }

        NumberAnimation on y {
            easing.type: Easing.OutElastic
            running: panel.show
            duration: 500
            from: panel.hiddenY
            to: panel.expandedY
        }

        NumberAnimation on opacity {
            easing.type: Easing.OutElastic
            running: !panel.show
            duration: 1000
            from: 0.7
            to: 0.3
        }

        NumberAnimation on opacity {
            easing.type: Easing.OutElastic
            running: panel.show
            duration: 1000
            from: 0.3
            to: 0.7
        }

        y: expandedY
        x: 0

        Rectangle {
            id: seekRect

            height: 10
            width: parent.width
            anchors {
                top: parent.top
            }
            color: "#476495"
            smooth: true

            Rectangle {
                id: seekBar

                property int position: 0

                width: Math.floor(
                           seekRect.width * (seekBar.position / mediaplayer.duration))
                height: seekRect.height
                anchors.bottom: seekRect.bottom
                color: "#14d4f1"
                smooth: true

                Behavior on position {
                    SmoothedAnimation {
                        velocity: 500
                        duration: 200
                    }
                }
            }

            Rectangle {
                width: 5
                height: seekRect.height
                color: "#ff9500"
                anchors {
                    bottom: seekBar.bottom
                    left: seekBar.left
                    right: undefined
                    leftMargin: seekBar.width - 2.5
                    verticalCenter: undefined
                }
            }

            MouseArea {
                id: seekMouseArea

                width: parent.width
                height: 10
                anchors.bottom: parent.bottom
                enabled: true
                onClicked: mediaplayer.seek(
                               Math.floor(
                                   (mouseX / seekRect.width) * mediaplayer.duration))
            }
        }

        Rectangle {
            id: controls
            color: "#000"
            width: parent.width
            height: 30

            anchors {
                bottom: parent.bottom
            }
        }
    }

    Timer {
        id: panelTimer
        interval: 4000
        running: false
        repeat: false
        onTriggered: {
            if (panel.show)
                panel.show = false
        }
    }
}
