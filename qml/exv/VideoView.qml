import QtQuick 2.0
import QtMultimedia 5.0
import QtGraphicalEffects 1.0

Rectangle {
    id: videoView
    anchors.fill: parent
    color: "#000"

    onHeightChanged: {
        panel.enabledAnimation = false
        panel.expandedY = rec.height - panel.height
        panel.hiddenY = rec.height - panel.hiddenMargin
        panel.y = rec.height - panel.height
        panel.enabledAnimation = true
    }

    function focusOnPlayer() {
        videoout.forceActiveFocus()
    }

    Item {
        //        visible: false;
        id: videoPlayerItem
        anchors.fill: parent
        property bool isPlaying: mediaplayer.playbackState == MediaPlayer.PlayingState // property to know if videoPlaying id ongoing or not
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
//            console.log('pos: '+mediaplayer.position);
//            console.log('total: '+mediaplayer.duration);
//            console.log('delta: ' + parseInt(mediaplayer.duration - mediaplayer.position));
            if (mediaplayer.playbackState == MediaPlayer.PlayingState) {
                mediaplayer.pause()
            } else {
                mediaplayer.play()
            }
        }

        function volUp() {
            mediaplayer.volume = mediaplayer.volume > 0.90 ? 1 : mediaplayer.volume + 0.1
        }

        function volDown() {
            mediaplayer.volume = mediaplayer.volume < 0.1 ? 0 : mediaplayer.volume - 0.1
        }

        function next() {
            currentFileIndex++
            sourceChanged()
        }

        function prev() {
            currentFileIndex--
            sourceChanged()
        }

        function goSearch() {
            mediaplayer.pause()
            videoView.visible = false
            videoView.enabled = false
            showPlaylist = false
            searchPanel.visible = true
            searchPanel.enabled = true
            searchPanel.focus2List()
        }

        function toggleFullscreen() {
            exv.toggleFullscreen()
        }

        function sourceChanged() {
            mediaplayer.stop()
            mediaplayer.source = ""
            mediaplayer.source = playlistModel.getItem(currentFileIndex).getUrl()
            subTitle = playlistModel.getItem(currentFileIndex).getName()
            mediaplayer.play()
        }

        function addToFav() {
//            console.log("Try to add: " + currentId)
            if(exv.addToFav(currentId))
                console.log("Added to fav")
            else
                console.log("Already in fav")
        }

        function getTime(val) {
            var seconds = val / 1000
            var h = Math.floor(seconds / 3600)
            var m = Math.floor(seconds / 60) - h*60
            var s = Math.floor(seconds) - h*60*60  - m*60
            var out = ""
            if(h > 0) {
                out += h < 10 ? "0"+h : h
                out += ":"
            }
            out += m < 10 ? "0"+m : m
            out += ":"
            out += s < 10 ? "0"+s : s
            return out
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
                videoPlayerItem.toggleFullscreen()
            }

            onPositionChanged: {
                if (!panel.show)
                    console.log("panel:" + panel.show)
                if (!panel.show)
                    panel.show = true
                if (panelTimer.running)
                    panelTimer.stop()

                if (mediaplayer.playbackState == MediaPlayer.PlayingState)
                    panelTimer.start()
            }

            cursorShape: (panel.show
                          || playlistPanel.show) ? Qt.ArrowCursor : Qt.BlankCursor
        }

        /*
                                                                                                               Actual QML based Video Component
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  */
        MediaPlayer {
            id: mediaplayer
            onPositionChanged: {
                seekBar.position = mediaplayer.position
            }

            onVolumeChanged: {
                console.log("volume changed: " + mediaplayer.volume)
                volumeBar.position = parseInt(mediaplayer.volume * 100)
            }

            onStatusChanged: {
                if(mediaplayer.status == MediaPlayer.EndOfMedia) {
                    var pos = mediaplayer.position
                    if((mediaplayer.duration - pos) > 500) {
                        console.log("Unexpected END OF MEDIA!");
                        mediaplayer.stop();
                        mediaplayer.play();
                        mediaplayer.seek(pos);
                    } else {
                        videoPlayerItem.next()
                    }
                }
            }

            onBufferProgressChanged: {
//                console.log("Buffer: "+bufferProgress)
                befferBar.val = bufferProgress
            }

        }

        VideoOutput {
            id: videoout
            anchors.fill: parent
            source: mediaplayer
            fillMode: VideoOutput.PreserveAspectFit
            focus: true
            Keys.onSpacePressed: videoPlayerItem.playPauseVideo()
            Keys.onLeftPressed: mediaplayer.seek(mediaplayer.position - 10000)
            Keys.onRightPressed: mediaplayer.seek(mediaplayer.position + 10000)

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
                if(!showPlaylist) {
                    videoPlayerItem.volUp()
                } else {
                    playlistPanel.listUp()
                }
            }

            Keys.onDownPressed: {
                if(!showPlaylist) {
                    videoPlayerItem.volDown()
                } else {
                    playlistPanel.listDown()
                }
            }

            Keys.onEscapePressed: {
                videoPlayerItem.goSearch()
            }

            Keys.onReturnPressed: {
                 if(!showPlaylist)
                    videoPlayerItem.toggleFullscreen()
                 else
                    playlistPanel.listSelect()

            }

        }

        Rectangle {
            id: befferBar
            anchors.horizontalCenter: parent.horizontalCenter
            y: 50
            width: bufferingText.width
            visible: befferBar.val < 1
            property double val: 1
            Text {
                id: bufferingText
                color: "#FFBF00"
                text: "Buffering: <b>" + parseInt(befferBar.val * 100) + "</b>%"
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
        width: parent.width
        height: rec.height - 200
        //        color: transparent
        anchors.centerIn: parent
        visible: mediaplayer.playbackState == MediaPlayer.PausedState
                 || mediaplayer.playbackState == MediaPlayer.StoppedState

        Column {
            Image {
                id: coverImage
                height: coverTitle.height - 100
                fillMode: Image.PreserveAspectFit
                width: coverTitle.width
                source: coverUrl
                anchors.leftMargin: coverTitle.width - 250
            }
            Text {
                id: firstTitle
                width: coverTitle.width
                color: "#fff"
                style: Text.Raised
                styleColor: "#000"
                text: mainTitle
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 26
                renderType: Text.NativeRendering
                wrapMode: Text.Wrap
            }
            Text {
                id: secondTitle
                width: coverTitle.width
                color: "#fff"
                style: Text.Raised
                styleColor: "#000"
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
        height: 70
        width: rec.width
        opacity: 0.7
        color: "transparent"

        property bool show: true
        property int hiddenMargin: 3
        property int expandedY: parent.height - panel.height
        property int hiddenY: parent.height - panel.hiddenMargin
        property bool enabledAnimation: true

        NumberAnimation on y {
            easing.type: Easing.OutCirc
            running: !panel.show && panel.enabledAnimation
            duration: 500
            from: panel.expandedY
            to: panel.hiddenY
        }

        NumberAnimation on y {
            easing.type: Easing.OutElastic
            running: panel.show && panel.enabledAnimation
            duration: 500
            from: panel.hiddenY
            to: panel.expandedY
        }

        NumberAnimation on opacity {
            easing.type: Easing.OutElastic
            running: !panel.show && panel.enabledAnimation
            duration: 1000
            from: 0.7
            to: 0.3
        }

        NumberAnimation on opacity {
            easing.type: Easing.OutElastic
            running: panel.show && panel.enabledAnimation
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
            color: videoPlayerItem.isPlaying && !panel.show ? "transparent" : "#476495"
            smooth: true

            Rectangle {
                id: seekBar
                width: Math.floor(
                           seekRect.width * (seekBar.position / mediaplayer.duration))
                height: seekRect.height
                anchors.bottom: seekRect.bottom
                color: videoPlayerItem.isPlaying && !panel.show ? "transparent" :  "#6BBAD3"
                smooth: true

                property int position: 0

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
            height: 60

            anchors {
                bottom: parent.bottom

            }
            ////////////////////////////////////////////////////////////////////

            Text {
                text: videoPlayerItem.getTime(mediaplayer.position)
                color: "#fff"
                font.pixelSize: 10
                renderType: Text.NativeRendering
            }

            Text {
                text: videoPlayerItem.getTime(mediaplayer.duration)
                color: "#fff"
                anchors.right: parent.right
                font.pixelSize: 10
                renderType: Text.NativeRendering
            }

            Rectangle {
                id: volumeRect
                x: 50

                height: 5
                width: 100
                anchors.verticalCenter: parent.verticalCenter
                color: "#476495"
                smooth: true
                radius: 8


                Rectangle {
                    id: volumeBar
                    radius: 8

                    property int position: parseInt(mediaplayer.volume * 100)

                    width: Math.floor(
                               volumeRect.width * (volumeBar.position / 100 ))
                    height: volumeRect.height
                    anchors.bottom: volumeRect.bottom
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
                    height: width
                    color: "#ff9500"
                    radius: 3
                    anchors {
                        bottom: undefined
                        left: volumeBar.left
                        right: undefined
                        leftMargin: volumeBar.width - width / 2
                        verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    id: volumeMouseArea

                    property double val: mouseX >= 0 && mouseX <= parent.width ? 1 / (parent.width / mouseX) : volumeMouseArea.val

                    width: parent.width
                    height: 10
                    anchors.bottom: parent.bottom
                    enabled: true
                    onClicked: mediaplayer.volume = val
                    onMouseXChanged: mediaplayer.volume = val
                    onWheel: {
                        wheel.angleDelta.y > 0 ? mediaplayer.volume += 0.1 : mediaplayer.volume -= 0.1
                    }
                }
            }

            ////////////////////////////////////////////////////////////////////

            Item {
                id: buttons
                width: buttonsRow.width
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    id: buttonsRow
                    spacing: 20
                    Button {
                        id: prevButton
                        imageSrc: "icons/backward.png"
                        onClicked: videoPlayerItem.prev()
                    }
                    Button {
                        id: playButton
                        imageSrc:  videoPlayerItem.isPlaying ? "icons/pause.png" : "icons/play.png"
                        onClicked: videoPlayerItem.playPauseVideo()
                    }
                    Button {
                        id: nextButton
                        imageSrc: "icons/forward.png"
                        onClicked: videoPlayerItem.next()
                    }
                }
            }

            Item {
                id: buttonsRight
                width: buttonsRightRow.width
                height: 32
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    id: buttonsRightRow
                    spacing: 20
                    Button {
                        id: toFavButton
                        imageSrc: "icons/bookmark.png"
                        onClicked: videoPlayerItem.addToFav()
                    }
                    Button {
                        id: searchButton
                        imageSrc: "icons/cat.png"
                        onClicked: videoPlayerItem.goSearch()
                    }
                    Button {
                        id: playlistButton
                        imageSrc: "icons/list.png"
                        onClicked: showPlaylist = !showPlaylist
                    }
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
}
