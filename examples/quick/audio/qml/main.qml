import QtQuick 2.0
import com.vk.api 1.0
import QtMultimedia 5.0

Rectangle {
    id: root

    width: 500
    height: 800

    TextEdit {
        id: login

        onLinkActivated: client.connectToHost()

        z: 10
        anchors.centerIn: parent
        text: qsTr("<a href=\"http://vk.com\">Click to login</a>")
        textFormat: TextEdit.RichText
    }

    Audio {
        id: player

        onStatusChanged: {
            switch (status) {
            case Audio.Stalled:
                break
            case Audio.Buffered:
                break
            case Audio.EndOfMedia:
                if (audioView.playingIndex === audioModel.count - 1)
                    audioView.playingIndex = 0
                else
                    audioView.playingIndex++;
            }
        }

        volume: 0.5
    }

    AudioModel {
        id: audioModel
        client: client
    }

    Client {
        id: client

        onMeChanged: {
            if (online) {
                client.me.update();
                audioModel.getAudio(client.me);
            }
        }

        OAuthConnection {
            id: conn

            Component.onCompleted: {
                setConnectionOption(Connection.ShowAuthDialog, true);
                setConnectionOption(Connection.KeepAuthData, true);
            }

            clientId: 3220807
            displayType: OAuthConnection.Popup
        }
    }

    ListView {
        id: audioView

        property int playingIndex: -1

        function play(index) {
            player.stop();
            player.source = audioModel.get(index, "url");
            player.play();
        }

        onPlayingIndexChanged: {
            if (playingIndex === -1)
                player.pause();
            else
                play(playingIndex);
        }

        anchors.fill: parent
        scale: 0
        model: audioModel

        header: Text {
            width: parent.width
            height: 50
            text: qsTr("%1 latest audio").arg(client.me ? client.me.name : qsTr("Unknown"))
            font.bold: true
            font.pixelSize: login.font.pixelSize * 1.1
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        delegate: Rectangle {
            id: item

            width: parent.width
            height: 80
            color: index % 2 ? syspal.alternateBase : "transparent"

            Image {
                id: preview

                source: "images/media-optical-audio.png"

                fillMode: Image.PreserveAspectFit
                clip: true
                smooth: true

                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: column.top
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: audioView.playingIndex = index;
                }
            }

            Column {
                id: column

                spacing: 2

                anchors {
                    left: preview.right
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                    leftMargin: 10
                    rightMargin: 10
                    topMargin: 10
                }

                Text {
                    id: titleLabel
                    width: parent.width
                    font.bold: true
                    text: artist
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                }
                Text {
                    id: descriptionLabel
                    width: parent.width
                    text: title
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                }

                Text {
                    id: durationLabel

                    color: syspal.dark
                    font.pixelSize: login.font.pixelSize * 0.8
                    width: parent.width
                    wrapMode: Text.Wrap
                    maximumLineCount: 1

                    text: qsTr("Duration %2").arg((duration / 60).toFixed(2))
                }
            }

            Rectangle {
                id: background
                color: "black"

                opacity: index === audioView.playingIndex ? 0.15 : 0

                height: 6
                radius: 6

                anchors {
                    bottom: hr.top
                    bottomMargin: 6
                    left: column.left
                    right: column.right
                }

                Rectangle {
                    id: progress

                    property real percentState: player.position / (duration * 1000)

                    color: "black"
                    height: parent.height

                    width: parent.width * percentState
                }
            }

            Rectangle {
                id: hr
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: syspal.window
            }
        }

        ScrollDecorator {
            flickableItem: parent
        }
    }

    SystemPalette {
        id: syspal
    }

    states: [
        State {
            name: "online"
            when: client.online
            PropertyChanges {
                target: login
                scale: 0
            }
            PropertyChanges {
                target: audioView
                scale: 1
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "online"
            NumberAnimation { target: audioView; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
            NumberAnimation { target: login; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
        }
    ]
}
